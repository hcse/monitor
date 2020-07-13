```
cleos create account eosio owner EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos create account eosio monitor EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos set contract monitor monitor/

cleos push action monitor newmetric '{
    "owner": "owner",
    "metric_name": "assetmetric",
    "meta": {
        "values":[]
    }
}' -p owner

cleos push action monitor newmetric '{
    "owner": "owner",
    "metric_name": "intmetric",
    "meta": {
        "values":[]
    }
}' -p owner


cleos push action monitor setvalue '["intmetric", ["int64", 3]]' -p owner
cleos push action monitor setvalue '["assetmetric", ["asset","1.00 HUSD"]]' -p owner

cleos get table monitor monitor metrics
cleos push action monitor increment '["intmetric"]' -p owner
cleos push action monitor increment '["assetmetric"]' -p owner

cleos push action monitor track '["assetmetric"]' -p owner
cleos push action monitor track '["intmetric"]' -p owner

cleos push action monitor add '["intmetric", ["int64", -2]]' -p owner
cleos push action monitor add '["assetmetric", ["asset","3.33 HUSD"]]' -p owner
```

Testnet info


eosc -u https://test.telos.kitchen --vault-file vault-test.json tx create mon1.hypha newmetric '{
    "owner": "metricmaker1",
    "metric_name": "banana",
    "meta": {
        "values":[]
    }
}' -p metricmaker1

eosc -u https://test.telos.kitchen --vault-file vault-test.json tx create mon1.hypha setvalue '{
    "metric_name": "banana",
    "value":["asset", "1.23 USD"]
}' -p metricmaker1

