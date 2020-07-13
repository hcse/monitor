package monitor

import (
	"context"
	"encoding/hex"
	"fmt"
	"log"
	"math/rand"
	"time"

	"github.com/eoscanada/eos-go"
	"github.com/eoscanada/eos-go/ecc"
	"github.com/eoscanada/eos-go/system"
)

const charset = "abcdefghijklmnopqrstuvwxyz" + "12345"
const creator = "eosio"
const defaultKey = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"
const repoHome = ".."
const testingEndpoint = "http://localhost:8888"

var testingKey ecc.PublicKey

// CreateRandoms ...
func CreateRandoms(ctx context.Context, api *eos.API, length int) ([]string, error) {

	i := 0
	var actions []*eos.Action
	var accounts []*eos.AccountName
	var accountNames []string
	accountNames = make([]string, length)
	accounts = make([]*eos.AccountName, length)
	keyBag := api.Signer

	for i < length {
		accountNameStr := randAccountName()
		acct := toAccount(accountNameStr, "random account name")
		key, _ := ecc.NewRandomPrivateKey()

		fmt.Println(key.String())
		err := keyBag.ImportPrivateKey(ctx, key.String())
		if err != nil {
			log.Panicf("import private key: %s", err)
		}

		accounts[i] = &acct
		accountNames[i] = accountNameStr
		actions = append(actions, system.NewNewAccount(creator, acct, key.PublicKey()))
		log.Println("Creating account: 	", acct, " with private key : ", key.String())
		i++
	}

	trxID, err := ExecTrx(ctx, api, actions)
	if err != nil {
		log.Panicf("cannot create random accounts: %s", err)
		return nil, err
	}
	log.Println("Created random accounts: ", trxID)
	return accountNames, nil
}

var seededRand *rand.Rand = rand.New(
	rand.NewSource(time.Now().UnixNano()))

func stringWithCharset(length int, charset string) string {
	b := make([]byte, length)
	for i := range b {
		b[i] = charset[seededRand.Intn(len(charset))]
	}
	return string(b)
}

// randAccountName returns a string for a random account name
func randAccountName() string {
	return stringWithCharset(12, charset)
}

// ExecTrx executes the slice of actions in a single transaction
func ExecTrx(ctx context.Context, api *eos.API, actions []*eos.Action) (string, error) {
	txOpts := &eos.TxOptions{}
	if err := txOpts.FillFromChain(ctx, api); err != nil {
		log.Printf("Error filling tx opts: %s", err)
		return "error", err
	}

	tx := eos.NewTransaction(actions, txOpts)
	_, packedTx, err := api.SignTransaction(ctx, tx, txOpts.ChainID, eos.CompressionNone)
	if err != nil {
		log.Printf("Error signing transaction: %s", err)
		return "error", err
	}

	response, err := api.PushTransaction(ctx, packedTx)
	if err != nil {
		log.Printf("Error pushing transaction: %s", err)
		return "error", err
	}
	trxID := hex.EncodeToString(response.Processed.ID)
	return trxID, nil
}

// SetContract sets the compiled WASM and ABI files to the provided account name
func SetContract(ctx context.Context, api *eos.API, accountName *eos.AccountName, wasmFile, abiFile string) (string, error) {
	setCodeAction, err := system.NewSetCode(*accountName, wasmFile)
	errorCheck("loading wasm file", err)

	setAbiAction, err := system.NewSetABI(*accountName, abiFile)
	errorCheck("loading abi file", err)

	return ExecTrx(ctx, api, []*eos.Action{setCodeAction, setAbiAction})
}
