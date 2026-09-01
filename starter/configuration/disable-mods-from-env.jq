def get_postfix: . as $item |
    if ($known_dlcs | index($item)) != null then "_DLC"
    elif ($features | index($item)) != null then ""
    else "_MOD" end;

def get_env_disable_key_by_mod_name: (. + (. | get_postfix)) | gsub("-"; "_") | ascii_upcase | ("DISABLE_" + .);

.mods |= map(
    if .name == "base" then .
    elif env[.name | get_env_disable_key_by_mod_name] == "true" then .enabled=false
    else . end
)