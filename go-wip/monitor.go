package monitor

import (
	"context"
	"encoding/json"
	"fmt"

	"github.com/eoscanada/eos-go"
)

// Metric object
type Metric struct {
	ID         eos.Name `json:"metric_name"`
	AssetValue eos.Asset
	IntValue   int64
	Metadata
}

// Metadata object
type Metadata struct {
	Names      map[string]eos.Name           `json:"names"`
	Strings    map[string]string             `json:"strings"`
	Assets     map[string]eos.Asset          `json:"assets"`
	TimePoints map[string]eos.BlockTimestamp `json:"time_points"`
	Ints       map[string]int64              `json:"ints"`
}

// Raw objects pulled from the blockchain
type Raw struct {
	ID         eos.Name      `json:"metric_name"`
	Names      []NameKV      `json:"names"`
	Strings    []StringKV    `json:"strings"`
	Assets     []AssetKV     `json:"assets"`
	TimePoints []TimePointKV `json:"time_points"`
	Ints       []IntKV       `json:"ints"`
}

// NameKV struct
type NameKV struct {
	Key   string   `json:"key"`
	Value eos.Name `json:"value"`
}

// StringKV struct
type StringKV struct {
	Key   string `json:"key"`
	Value string `json:"value"`
}

// AssetKV struct
type AssetKV struct {
	Key   string    `json:"key"`
	Value eos.Asset `json:"value"`
}

// TimePointKV struct
type TimePointKV struct {
	Key   string             `json:"key"`
	Value eos.BlockTimestamp `json:"value"`
}

// IntKV struct
type IntKV struct {
	Key   string `json:"key"`
	Value int64  `json:"value"`
}

// Metric converts a Raw object to a Metric object
func toMetric(raw Raw) Metric {
	var metric Metric
	metric.Names = make(map[string]eos.Name)
	for index, element := range raw.Names {
		metric.Names[element.Key] = raw.Names[index].Value
	}

	metric.Assets = make(map[string]eos.Asset)
	for index, element := range raw.Assets {
		metric.Assets[element.Key] = raw.Assets[index].Value
	}

	metric.TimePoints = make(map[string]eos.BlockTimestamp)
	for index, element := range raw.TimePoints {
		metric.TimePoints[element.Key] = raw.TimePoints[index].Value
	}

	metric.Ints = make(map[string]int64)
	for index, element := range raw.Ints {
		metric.Ints[element.Key] = raw.Ints[index].Value
	}

	metric.Strings = make(map[string]string)
	for index, element := range raw.Strings {
		metric.Strings[element.Key] = raw.Strings[index].Value
	}
	metric.ID = raw.ID
	return metric
}

// Metrics returns the list of all metrics from the monitor contract
func Metrics(ctx context.Context, api *eos.API, contract string) []Metric {
	var objects []Raw
	var request eos.GetTableRowsRequest
	request.Code = contract
	request.Scope = contract
	request.Table = "metrics"
	request.Limit = 1000
	request.JSON = true
	response, _ := api.GetTableRows(ctx, request)
	response.JSONToStructs(&objects)

	var metrics []Metric
	metrics = make([]Metric, len(objects))
	for index, rawObject := range objects {
		metrics[index] = toMetric(rawObject)
	}
	return metrics
}

type newMetricParam struct {
	Owner      eos.AccountName `json:"owner"`
	MetricName eos.Name        `json:"metric_name"`
	Meta       Metadata        `json:"meta"`
}

// NewMetric creates a new metric in the monitor contract
func NewMetric(ctx context.Context, api *eos.API, monitorContract, metricOwner, metricNameStr string) {

	contract := toAccount(monitorContract, "contract")
	action := toActionName("newmetric", "action")
	//	metricName, _ := eos.StringToName(metricNameStr)

	// meta := Metadata{}
	// metaJ, _ := json.MarshalIndent(meta, "", " ")

	nmp := newMetricParam{
		Owner:      toAccount(metricOwner, "metric owner"),
		MetricName: eos.Name(metricNameStr),
		Meta:       Metadata{},
	}

	nmpj, _ := json.MarshalIndent(nmp, "", "  ")
	fmt.Println(string(nmpj))

	// fmt.Println(string(metaJ))
	actionData := eos.NewActionData(nmp)

	fmt.Println(actionData)
	actions := []*eos.Action{
		{
			Account: contract,
			Name:    action,
			Authorization: []eos.PermissionLevel{
				{Actor: eos.AN(metricOwner), Permission: eos.PN("active")},
			},
			ActionData: actionData,
		}}

	actionJ, _ := json.MarshalIndent(actions, "", " ")
	fmt.Println(string(actionJ))
	trxID, err := ExecTrx(ctx, api, actions)
	if err != nil {
		panic(err)
	}
	fmt.Println(trxID)
}
