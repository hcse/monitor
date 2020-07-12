# Purpose
Smart contract to manage metrics and events. 

This metrics functionality is designed to record on-chain metrics to be used with prometheus and grafana.

The events functionality is designed to create a 'feed' of relevant events for dapp participants, which can be much more tailored than a typical block explorer transaction history.


--- monitor Project ---

 - How to Build -
   - cd to 'build' directory
   - run the command 'cmake ..'
   - run the command 'make'

 - After build -
   - The built smart contract is under the 'monitor' directory in the 'build' directory
   - You can then do a 'set contract' action with 'cleos' and point in to the './build/monitor' directory

 - Additions to CMake should be done to the CMakeLists.txt in the './src' directory and not in the top level CMakeLists.txt


cleos create account eosio owner EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos create account eosio monitor EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos set contract monitor monitor/

cleos push action monitor newmetric '{
    "owner": "owner",
    "metric_name": "assetmetric",
    "meta": {
        "names":[],
        "strings":[],
        "assets":[],
        "time_points":[],
        "ints":[],
        "trxs":[]
    }
}' -p owner

cleos push action monitor newmetric '{
    "owner": "owner",
    "metric_name": "intmetric",
    "meta": {
        "names":[],
        "strings":[],
        "assets":[],
        "time_points":[],
        "ints":[],
        "trxs":[]
    }
}' -p owner


cleos push action monitor setvalue '["intmetric", ["int64", 3]]' -p owner
cleos push action monitor setvalue '["assetmetric", ["asset","1.00 HUSD"]]' -p owner

cleos get table monitor monitor metrics
cleos push action monitor increment '["intmetric"]' -p owner
cleos push action monitor increment '["assetmetric"]' -p owner

cleos push action monitor add '["intmetric", ["int64", -2]]' -p owner
cleos push action monitor add '["assetmetric", ["asset","3.33 HUSD"]]' -p owner
