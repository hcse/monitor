package monitor

import (
	"context"
	"fmt"
	"log"
	"testing"

	"github.com/eoscanada/eos-go"
)

var api *eos.API
var monitorContract eos.AccountName
var monitorContractName string
var ctx context.Context

func init() {
	ctx = context.Background()
	api = eos.New("http://localhost:8888")
	api.Debug = true
	keyBag := &eos.KeyBag{}
	err := keyBag.ImportPrivateKey(ctx, defaultKey)
	if err != nil {
		log.Panicf("cannot import default private key: %s", err)
	}
	api.SetSigner(keyBag)

	accounts, _ := CreateRandoms(ctx, api, 1)
	monitorContractName = accounts[0]
	monitorContract = eos.AN(accounts[0])

	SetContract(ctx, api, &monitorContract, "../monitor/monitor.wasm", "../monitor/monitor.abi")
}

func TestGetMetrics(t *testing.T) {

	NewMetric(ctx, api, monitorContractName, monitorContractName, "votecount")
	metrics := Metrics(ctx, api, monitorContractName)
	fmt.Println(metrics)
}
