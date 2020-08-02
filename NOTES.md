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


eosc -u https://testnet.telos.caleos.io --vault-file vault-test.json tx create mon1.hypha event '{
    "broadcaster": "metricmaker1",
    "level":"high",
    "values":[
        {"key":"Event Type", "value":["string", "New Proposal"]},
        {"key":"Event Title", "value":["string", "Underwater basketweaver assignment proposal"]},
        {"key":"Proposal Type", "value":["name", "assignment"]},
        {"key":"Role", "value":["string", "Underwater basketweaver"]},
        {"key":"Weekly HUSD Salary", "value":["asset", "100.00 HUSD"]},
        {"key":"Weekly SEEDS Salary", "value":["asset", "12000.0000 SEEDS"]},
        {"key":"Proposer", "value":["name", "hyphanewyork"]}, 
        {"key":"Description", "value":["string", "Etiam aliquet ut lectus at congue. Suspendisse blandit at tellus sit amet pellentesque. Phasellus ex felis, venenatis in tincidunt ultrices, hendrerit ac lectus. Proin ante dui, ullamcorper nec dapibus nec, mattis id massa. Ut mattis pellentesque urna, et facilisis dui finibus quis. Nunc auctor eu nunc sed vulputate. Pellentesque ornare ultricies arcu id luctus. Nulla quis diam vitae diam accumsan lacinia. Nam eget ante in enim lobortis facilisis. Duis sodales ultrices ligula et tristique. Nunc maximus sagittis nunc et blandit. "]}
    ]
}' -p metricmaker1



