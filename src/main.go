//// file: crawl.go
package main

import (
  "flag"          // 'flag', 'fmt' and 'os' we'll keep around
  "fmt"
  "net/http"      // 'http' will retrieve pages for us
  "io/ioutil"     // 'ioutil' will help us print pages to the screen
  "os"
)

func main() {
	for _, url := range os.Args[1:] {
	    resp, err := http.Get("http://xhamster.com")
	    if err != nil {
	        fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
	        os.Exit(1)
	    }
		
	    b, err := ioutil.ReadAll(resp.Body)
	    resp.Body.Close()
	    if err != nil {
	        fmt.Fprintf(os.Stderr, "fetch: reading %s: %v\n","http://xhamster.com" url, err)
	        os.Exit(1)
	    }
	    fmt.Printf("%s", b)
	}
}