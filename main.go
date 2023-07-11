package main

import (
	"fmt"
	"log"
	"net/http"
	"net/url"

	"github.com/cretz/bine/tor"
)

func main() {
	// Start a new Tor proxy connection
	proxyURL, err := url.Parse("socks5://localhost:9050")
	if err != nil {
		log.Fatal(err)
	}
	dialer, err := tor.NewDialer(proxyURL)
	if err != nil {
		log.Fatal(err)
	}

	// Create an HTTP client with the Tor proxy dialer
	httpClient := &http.Client{
		Transport: &http.Transport{
			Dial: dialer.Dial,
		},
	}

	// Make a request through Tor
	response, err := httpClient.Get("http://tortaxi.onion")
	if err != nil {
		log.Fatal(err)
	}
	defer response.Body.Close()

	// Print the response status code
	fmt.Println("Response Status:", response.Status)
}
