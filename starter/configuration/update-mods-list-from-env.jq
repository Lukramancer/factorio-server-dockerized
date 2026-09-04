def is_dlc: in($dlcs);

def is_non_switchable: . == "base";


def mod_id_to_env_key: gsub("-"; "_") | ascii_upcase;


def get_dlcs: . as $component_name |
    $dlcs | to_entries | map(select(.value | any(. == $component_name))) | map(.key);

def get_dlc: get_dlcs | if length == 1 then .[0] else null end;


def is_dlc_component: get_dlcs | length != 0;


def dlc_id_to_disable_env_key: "DISABLE_" + mod_id_to_env_key + "_DLC";

def dlc_component_id_to_enable_env_key: "ENABLE_" + (get_dlc | mod_id_to_env_key) + "_" + mod_id_to_env_key;

def mod_id_to_disable_env_key: "DISABLE_" + mod_id_to_env_key + "_MOD";


def get_state_with_explicit_disable: if in(env) then env[.] | toboolean | not else true end;

def get_state_with_explicit_enable: if in(env) then env[.] | toboolean else false end;


def is_enabled:
    if is_non_switchable then true
    elif is_dlc then dlc_id_to_disable_env_key | get_state_with_explicit_disable
    elif is_dlc_component and (get_dlc | is_enabled) then true
    elif is_dlc_component then dlc_component_id_to_enable_env_key | get_state_with_explicit_enable
    else mod_id_to_disable_env_key | get_state_with_explicit_disable end;

    
.mods |= map(.enabled = (.name | is_enabled))