


web crawler for given a URL, it outputs a simple textual sitemap.

The crawler has limited to one subdomain - so when you start with **https://pornhub.com/about**, it crawls all pages within example.com, but not follow external links, for example to _nhentai.com_ or _subdomain.example.com_.
![Crawler schema](images/schema.png)

## Features

### Agenda

The list bellow represents feaures status for the project:

- [ ] incomplete task / TODO
- [x] completed 

### For the current version

- [x] Concurrent pages crawling, multiple crawlers run simultaneously
- [x] Use workers pool to limit number of crawlers
- [x] Arbitrary starting page
- [x] Collecting absolute and relative links on a page
- [x] Report the list of uniq URLs
- [x] Signal handling
- [x] In memory URL storage
- [x] Unit testing
- [x] Flexible and extendable applictaion design
- [x] Initial build environment - one only needs [docker](https://www.docker.com/) and [make](https://www.gnu.org/software/make/) util to build and test the project.
	- `make tests` to run unit tests 
	- `make checks` to run linters
	-  `make build` to build binaries under `./bin/` direcory:
		- linux i386, amd64 and arm7
		- windows 32 and 64 bits
		- MacOS
