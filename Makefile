IMG_NAME = elzar
CONTAINER_NAME = elzar_container

DOCKER = sudo docker
DOCKER_RUN = $(DOCKER) run -it -v `pwd`/data:/data

.PHONY: all build run clean stop clean_all

all: build

build:
	@$(DOCKER) build --rm=true -t $(IMG_NAME) .

run:
	@$(DOCKER_RUN) --name=$(CONTAINER_NAME) $(IMG_NAME)

start:
	@$(DOCKER) start -i $(CONTAINER_NAME)

stop:
	@$(DOCKER) stop $(CONTAINER_NAME)

deploy:
	@$(DOCKER) tag -f $(IMG_NAME) tudinfse/elzar
	@$(DOCKER) push tudinfse/elzar

clean:
	@$(DOCKER) rm $(CONTAINER_NAME)

clean_all:
	@$(DOCKER) rm $(CONTAINER_NAME)
	@$(DOCKER) rmi $(IMG_NAME)

