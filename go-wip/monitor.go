package monitor

import (
	"context"

	"github.com/eoscanada/eos-go"
)

// Metric represents a metric tracked in the monitor contract
// the struct code was derived from https://mholt.github.io/json-to-go/
type Metric struct {
	MetricName string `json:"metric_name"`
	Metadata   struct {
		Values []struct {
			Key   string   `json:"key"`
			Value []string `json:"value"`
		} `json:"values"`
	} `json:"metadata"`
	Value []string `json:"value"`
}

// Metrics returns the list of all metrics from the monitor contract
func Metrics(ctx context.Context, api *eos.API, contract string) []Metric {
	var objects []Metric
	var request eos.GetTableRowsRequest
	request.Code = contract
	request.Scope = contract
	request.Table = "metrics"
	request.Limit = 1000
	request.JSON = true
	response, _ := api.GetTableRows(ctx, request)
	response.JSONToStructs(&objects)
	return objects
}
