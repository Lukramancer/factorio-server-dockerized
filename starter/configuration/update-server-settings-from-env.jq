def is_not_comment: if type == "string" then .[0:9] != "_comment_" else true end;

def get_env_key: "SERVER_" + (map(ascii_upcase) | join("_"));

def get_object(deserializer; default): get_env_key as $env_key |
    if $env_key | in(env) then (env[$env_key] | deserializer)
    else default end;

def extract(reference_value): get_object(reference_value | get_deserializer; reference_value);

def is_not_within_array: all(type != "number");

def process_path($object): . as $path |
    $object | getpath($path) as $previous_value |
    if $previous_value | type == "string" then $path | get_object(tostring; $previous_value)
    elif $previous_value | type == "boolean" then $path | get_object(toboolean; $previous_value)
    elif $previous_value | type == "number" then $path | get_object(tonumber; $previous_value)
    elif ($previous_value | type == "array") and ($previous_value.[0] | type == "string") then $path | get_object(split("\\s*,\\s*"; null); $previous_value)
    else halt_error end;

def produce_config_updates: . as $config |
    [paths] |
    map(select(all(is_not_comment))) |
    map(select(is_not_within_array)) |
    map(select(. as $path | $config | getpath($path) | type != "object")) |
    map(. as $path | {} | setpath($path; $path | process_path($config)));

reduce produce_config_updates.[] as $config_update (.; . * $config_update)