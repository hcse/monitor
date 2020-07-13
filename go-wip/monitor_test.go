package monitor

import (
	"context"
	"fmt"
	"testing"

	"github.com/eoscanada/eos-go"
)

var api *eos.API
var ctx context.Context

func init() {
	ctx = context.Background()
	api = eos.New("http://localhost:8888")
	api.Debug = true
}

func TestGetMetrics(t *testing.T) {
	metrics := Metrics(ctx, api, "monitor")
	fmt.Println(metrics)
}
