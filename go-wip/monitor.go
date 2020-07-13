package monitor

import (
	"context"
	"fmt"

	"github.com/eoscanada/eos-go"
)

const (
	MetadataVariantType = iota
)

// MetadataVariant may hold a name, int64, asset, string, or time_point
type MetadataVariant struct {
	eos.BaseVariant
}

var metadataVariantFactoryImplMap = map[uint32]eos.VariantImplFactory{
	eos.Name: func() interface{} { return new(MetadataVariant) }, // ??
	

}

func (mv *MetadataVariant) UnmarshalBinary(decoder *eos.Decoder) error {
	return mv.BaseVariant.UnmarshalBinaryVariant(decoder, metadataVariantFactoryImplMap)
}

type Metadata
// Metadata object
type Metadata struct {
	Values map[string]MetadataVariant `json:"values"`
}

// Metric represents a metric tracked in the monitor contract
type Metric struct {
	MetricName      eos.Name        `json:"metric_name"`
	MertricMetadata Metadata        `json:"metadata"`
	Value           eos.BaseVariant `json:"value"`
}

// Metrics returns the list of all metrics from the monitor contract
func Metrics(ctx context.Context, api *eos.API, contract string) []*Metric {
	var objects []*Metric
	var request eos.GetTableRowsRequest
	request.Code = contract
	request.Scope = contract
	request.Table = "metrics"
	request.Limit = 1000
	request.JSON = true
	response, err := api.GetTableRows(ctx, request)
	if err != nil {
		fmt.Println("Error with GetTableRows: ", err)
		return nil
	}

	fmt.Println(string(response.Rows))
	err = response.JSONToStructs(&objects)
	if err != nil {
		fmt.Println("Error with BinaryToStructs: ", err)
		return nil
	}

	return objects
}
