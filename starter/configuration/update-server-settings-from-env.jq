def is_comment: .key[0:9] == "_comment_";

def get_object(type): ("SERVER_" + (.key | ascii_upcase)) as $env_key | if $env_key | in(env) then (.value = (env[$env_key] | type)) else . end;

with_entries(
    if is_comment then .
    elif (.value | type == "string") then get_object(tostring)
    elif (.value | type == "number") then get_object(tonumber)
    elif (.value | type == "boolean") then get_object(toboolean)
    elif ((.value | type == "array") and (.value[0] | type == "string")) then get_object(split("\\s*,\\s*"; null))
    elif ((.value | type == "array") and (.value[0] | type == "number")) then get_object(split("\\s*,\\s*"; null) | tonumber)
    else . end
)