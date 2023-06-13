package main

import (
	"fmt"
	"net/http"
	"io/ioutil" 
	"net/http"
    "net/url"  // We're going to use the standard library
    "os"

)



var visited = make(map[string]bool)






func main() {
	var s, sep string
    for i := 1; i < len(os.Args); i++ {
        s += sep + os.Args[i]
        sep = " "
    }
    fmt.Println(s)
}
	// URL to crawl
	 type URL := "https://wired.com"

	// Send a GET request to the URL
	resp, err := http.Get(url)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer resp.Body.Close()

	// Parse the response as HTML
	doc, err := html.Parse(resp.Body)
	if err != nil {
		fmt.Println(err)
		return
	}

	// Traverse the HTML tree and extract all the links
	var links []string
	var f func(*html.Node)
	f = func(n *html.Node) {
		if n.Type == html.ElementNode && n.Data == "a" {
			for _, a := range n.Attr {
				if a.Key == "href" {
					links = append(links, a.Val)
					break
				}
			}
		}
		for c := n.FirstChild; c != nil; c = c.NextSibling {
			f(c)
		}
	}
	f(doc)

	// Print the links
	for _, link := range links {
		fmt.Println(link)
	}
}