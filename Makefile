APP_NAME := $(shell basename "$(CURDIR)")
SHELL := /bin/bash

VERSION ?= $(shell git describe --tags  2>/dev/null || echo -n "untagged")
BRANCH ?= $(shell git rev-parse --abbrev-ref HEAD)
COMMIT ?= $(shell git rev-parse --short HEAD)
DATE ?= $(shell date +'%d/%m/%Y')

default: all

clean:
	@rm -rf ./.cover/*.{txt,coverage}
	@rm -rf ./bin/*

build-image: clean
	docker build --rm -t $(APP_NAME)/builder . -f ./build/Dockerfile

build: build-image
	docker run --rm -e version=$(VERSION) -e branch=$(BRANCH) -e commit=$(COMMIT) -e date=$(DATE) -e appname=$(APP_NAME) -v "$(CURDIR):/src/" $(APP_NAME)/builder

test-cover-prep: 
	@test -d ./.cover && rm -Rf ./.cover/* || mkdir -p ./.cover
	docker build --rm -t $(APP_NAME)/tester . -f ./tests/Dockerfile

test-cover: test-cover-prep
	docker run  --rm -v "$(CURDIR):/$(APP_NAME)" $(APP_NAME)/tester /bin/bash -c './scripts/run-tests.sh .'

tests: test-cover-prep test-cover
	open ./.cover/unit.html

checks: test-cover-prep
	docker run  --rm -v "$(CURDIR):/$(APP_NAME)" $(APP_NAME)/tester /bin/bash -c './scripts/run-linter.sh .'

all: clean tests checks build-image build 

.PHONY: build tests
