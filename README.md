# SyncUp

SyncUp is a lightweight tool designed to keep a destination folder updated by selectively copying only the updated files from a source folder. This project was created because I found rsync challenging to use for my specific needs.

## Features

* Selectively copies updated files from a source folder to a destination folder.
* Simple and straightforward usage.

## How to use

Clone de repo:

```
git clone https://github.com/your/repository.git
cd SyncUp
```

Then build:

```
cmake . -B build
cmake --build build
```

Install:
```
sudo cmake --install build
```

## Why SyncUp?

I developed SyncUp because I don't know how to use `rsync`.



