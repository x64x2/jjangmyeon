// // file: retrieve.go
package main // Note: this is a new temporary file, not our crawl.go

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func rtr() {
	resp, err := http.Get("http://6brand.com.com")

	fmt.Println("http transport error is:", err)

	body, err := ioutil.ReadAll(resp.Body)

	fmt.Println("read error is:", err)

	fmt.Println(string(body))
}
