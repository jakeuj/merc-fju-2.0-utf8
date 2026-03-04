IMAGE ?= merc-fju
DOCKER ?= docker

.PHONY: docker-build docker-run docker-shell docker-clean

docker-build:
	$(DOCKER) build -t $(IMAGE) -f docker/Dockerfile .

docker-run:
	$(DOCKER) run --rm -it \
		-p 13838:3838 -p 11234:1234 -p 18888:8888 \
		-v $$(pwd):/app \
		--name $(IMAGE) \
		$(IMAGE)

docker-shell:
	$(DOCKER) run --rm -it \
		-p 13838:3838 -p 11234:1234 -p 18888:8888 \
		-v $$(pwd):/app \
		--entrypoint /bin/bash \
		$(IMAGE)

docker-clean:
	-$(DOCKER) rm -f $(IMAGE) >/dev/null 2>&1 || true
