# Web Crawler in Go

This is a simple web crawler implemented in Go. It takes a seed URL and a maximum depth as input and then starts crawling the web from the seed URL, following links up to the specified maximum depth.

## Usage

To use the web crawler, first install Go on your machine and set up your Go workspace.

Then, clone this repository and navigate to the root directory of the project:

> $ git clone https://github.com/ericsomto/wbcrl

> $ cd wbcrl

Next, build the program using the following command:

> $ go build

This will create an executable file called web-crawler. You can then run the web crawler using the following command:

> $ ./wbclr -seed https://www.wired.com -depth 3

This will start the web crawler at the seed URL https://www.example.com and crawl up to a depth of 3. You can adjust the seed URL and maximum depth to your liking.

### Output

The web crawler will output the URLs of the pages it has visited, one URL per line. You can redirect the output to a file if you want to save the results:

> $ ./wbclr -seed https://www.wired.com -depth 3 > results.txt

### Dependencies

This web crawler uses the following third-party packages:

    golang.org/x/net/html for parsing HTML pages
    golang.org/x/net/publicsuffix for determining the public suffix of a host

### Contribute

If you want to contribute to this project, please fork the repository and submit a pull request. Any contributions, whether they are bug fixes or new features, are welcome.

