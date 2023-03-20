// // file: crawl.go
package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
)

func crawl() {
	flag.Parse()

	args := flag.Args()
	fmt.Println(args)
	if len(args) < 1 {
		fmt.Println("Please specify start page")
		os.Exit(1)
	}
	retrieve(args[0])
}
func retrieve(uri string) {

	resp, err := http.Get(uri)
	if err != nil {
		return
	}
	defer resp.Body.Close()

	body, _ := ioutil.ReadAll(resp.Body)

	fmt.Println(string(body)) // if I name it and don't use it
}
