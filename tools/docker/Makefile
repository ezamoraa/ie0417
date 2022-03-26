# (C) Copyright 2020-2022 Hewlett Packard Enterprise Development LP.

.PHONY: image publish

DOCKER_IMAGE=ezamoraa/ie0417
VERSION=$(shell cat version)
REGISTRY=docker.io

image:
	DOCKER_BUILDKIT=1 docker build --build-arg http_proxy=${http_proxy} --build-arg https_proxy=${https_proxy} . -t ${DOCKER_IMAGE}:$(VERSION)
	docker tag ${DOCKER_IMAGE}:$(VERSION) ${DOCKER_IMAGE}:latest

publish: image
	docker tag ${DOCKER_IMAGE}:$(VERSION) ${REGISTRY}/${DOCKER_IMAGE}:$(VERSION)
	docker push ${REGISTRY}/${DOCKER_IMAGE}:$(VERSION)
