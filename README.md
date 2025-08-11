# Orbis
A drone using SLAM to map its surroundings, all controlled using a ROG Ally.

## Build
Build the `main` service image:
```bash
docker compose build main
```

## Down
Stop and remove containers, networks, and orphans:
```bash
docker compose down --remove-orphans
```

## Run
Run the main service interactively (removes container after exit):
```bash
docker compose run --rm main
```

## Prune
Nuke everything unused:
```bash
docker system prune -a --volumes
```

## Force Stop
Kill docker:
```bash
docker kill $(docker ps -q)
```