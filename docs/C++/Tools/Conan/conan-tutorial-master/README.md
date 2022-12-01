# Conan tutorial

## Usage

### Step 1

Download the repository code of conan-tutorial
```sh
git clone ssh://git@g.hz.netease.com:22222/nasdaq/im-cross-project/conan-tutorial.git
```

### Step 2

Installation dependencies
```sh
cd conan-tutorial
yarn # or npm i
```

### Step 3

Start the project, then you can modify the configuration and write document content according to your needs
```sh
npm run docs:dev
```

### Step 4

Packaged project
```sh
npm run docs:build
```
As a result, a `dist` folder will be generated in the `docs/.vuepress/` directory, which contains the packaged code
