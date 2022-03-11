# build chromium

## docs

### doc [Get the Code: Checkout, Build, & Run Chromium](https://www.chromium.org/developers/how-tos/get-the-code/)

> Chromium uses [Ninja](https://ninja-build.org/) as its main build tool along with a tool called [GN](https://gn.googlesource.com/gn/+/main/docs/quick_start.md) to generate `.ninja` files. You can create any number of *build directories* with different configurations. 



### doc [Checking out and building Chromium for Mac](https://chromium.googlesource.com/chromium/src/+/main/docs/mac_build_instructions.md)

```sh
caffeinate fetch --no-history chromium
```

> NOTE:
>
> 这种下载方式最快



### doc [Checking out and building Chromium on Linux](https://chromium.googlesource.com/chromium/src/+/main/docs/linux/build_instructions.md)



### doc [Checking out and building Chromium for iOS](https://chromium.googlesource.com/chromium/src/+/main/docs/ios/build_instructions.md)



### doc [GN build configuration](https://www.chromium.org/developers/gn-build-configuration/)



### doc [Common Build Tasks](https://chromium.googlesource.com/chromium/src/+/778a7e84f65fd36658f91881bda07b9153a8729d/docs/common_build_tasks.md)



## 实践

通过 [Get the Code: Checkout, Build, & Run Chromium](https://www.chromium.org/developers/how-tos/get-the-code/) 中的内容可知: 通过执行 `fetch` 来下载source code 和 dependency。



### `fetch.py`

Checkout type map

```shell
fetch --no-history chromium
```

#### `handle_args`

```python
options, config, props = handle_args(sys.argv)
```

输出值如下:

```
config:chromium
options:{'dry_run': False, 'nohooks': False, 'no_history': True, 'force': False}
props:[]
```

#### `run_config_fetch`

`spec, root = run_config_fetch(config, props)` 运行的时候，config的值是 `chromium`。查看 `run_config_fetch` 的source code:

一、它会执行 `fetch_configs/chromium.py`

二、它的返回值如下:

```
spec:{'type': 'gclient_git', 'gclient_git_spec': {'solutions': [{'name': 'src', 'url': 'https://chromium.googlesource.com/chromium/src.git', 'managed': False, 'custom_deps': {}, 'custom_vars': {}}]}}
root:src
```

`run(options, spec, root)`

可以看到，它的 `checkout_type` 是 `gclient_git`



### [`gclient.py`](https://chromium.googlesource.com/chromium/tools/depot_tools.git/+/refs/heads/main/gclient.py)



#### hooks

如何来探索 `gclient` 的hook呢？在阅读 [Checking out and building Chromium on Linux](https://chromium.googlesource.com/chromium/src/+/main/docs/linux/build_instructions.md) 中为注意到它在执行fetch的时候特地指定不执行hook，然后再次执行hook，我按照它描述的方式进行了执行: 

```shell
fetch --no-history --nohooks chromium
```

`gclient runhooks`，输出如下:

```sh
Running hooks:  13% (16/121) mac_toolchain 
________ running 'python3 src/build/mac_toolchain.py' in '/Users/dengkai01/chromium2'
Skipping Mac toolchain installation for mac


Running hooks:  14% (18/121) clang               
________ running 'python3 src/tools/clang/scripts/update.py' in '/Users/dengkai01/chromium2'
update.py
Downloading https://commondatastorage.googleapis.com/chromium-browser-clang/Mac/clang-llvmorg-15-init-1995-g5bec1ea7-1.tgz .......... Done.
Hook 'python3 src/tools/clang/scripts/update.py' took 12.08 secs


Running hooks:  19% (24/121) objdump       
________ running 'python3 src/tools/clang/scripts/update.py --package=objdump' in '/Users/dengkai01/chromium2'
update.py
Downloading https://commondatastorage.googleapis.com/chromium-browser-clang/Mac/llvmobjdump-llvmorg-15-init-1995-g5bec1ea7-1.tgz .......... Done.


Running hooks:  23% (29/121) clang_format_mac 
________ running 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --no_auth --bucket chromium-clang-format -s src/buildtools/mac/clang-format.sha1' in '/Users/dengkai01/chromium2'
0> Downloading src/buildtools/mac/clang-format@5ba974b3b37f9f4e3b44fdde11d7ef2ab71619ab...
Downloading 1 files took 42.345282 second(s)
Hook 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --no_auth --bucket chromium-clang-format -s src/buildtools/mac/clang-format.sha1' took 42.43 secs


Running hooks:  28% (34/121) test_fonts        
________ running 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --extract --no_auth --bucket chromium-fonts -s src/third_party/test_fonts/test_fonts.tar.gz.sha1' in '/Users/dengkai01/chromium2'
0> Downloading src/third_party/test_fonts/test_fonts.tar.gz@cd96fc55dc243f6c6f4cb63ad117cad6cd48dceb...
0> Extracting 33 entries from src/third_party/test_fonts/test_fonts.tar.gz to src/third_party/test_fonts/test_fonts
Downloading 1 files took 11.810320 second(s)
Hook 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --extract --no_auth --bucket chromium-fonts -s src/third_party/test_fonts/test_fonts.tar.gz.sha1' took 11.91 secs
Hook 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_auth --quiet --bucket chromium-webrtc-resources -d src/third_party/opus/tests/resources' took 10.83 secs


Running hooks:  32% (39/121) wasm_fuzzer         
________ running 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --no_auth -u --bucket v8-wasm-fuzzer -s src/v8/test/fuzzer/wasm_corpus.tar.gz.sha1' in '/Users/dengkai01/chromium2'
0> Downloading src/v8/test/fuzzer/wasm_corpus.tar.gz@5e785f96e6a9bb5a1fb54cc8649a551569aeb31d...
0> Extracting 53780 entries from src/v8/test/fuzzer/wasm_corpus.tar.gz to src/v8/test/fuzzer/wasm_corpus
Downloading 1 files took 22.737666 second(s)
Hook 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --no_auth -u --bucket v8-wasm-fuzzer -s src/v8/test/fuzzer/wasm_corpus.tar.gz.sha1' took 22.82 secs


Running hooks:  34% (42/121) node_mac_arm64
________ running 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --extract --no_auth --bucket chromium-nodejs/16.13.0 -s src/third_party/node/mac/node-darwin-arm64.tar.gz.sha1' in '/Users/dengkai01/chromium2'
0> Downloading src/third_party/node/mac/node-darwin-arm64.tar.gz@31859fc1fa0994a95f44f09c367d6ff63607cfde...
0> Extracting 3247 entries from src/third_party/node/mac/node-darwin-arm64.tar.gz to src/third_party/node/mac/node-darwin-arm64
Downloading 1 files took 13.792733 second(s)
Hook 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --extract --no_auth --bucket chromium-nodejs/16.13.0 -s src/third_party/node/mac/node-darwin-arm64.tar.gz.sha1' took 13.90 secs


Running hooks:  36% (44/121) webui_node_modules
________ running 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --extract --no_auth --bucket chromium-nodejs -s src/third_party/node/node_modules.tar.gz.sha1' in '/Users/dengkai01/chromium2'
0> Downloading src/third_party/node/node_modules.tar.gz@aad6877c8cd3cff0ef1ba628d71acc981c520c29...
0> Extracting 8650 entries from src/third_party/node/node_modules.tar.gz to src/third_party/node/node_modules
Downloading 1 files took 12.500377 second(s)
Hook 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --extract --no_auth --bucket chromium-nodejs -s src/third_party/node/node_modules.tar.gz.sha1' took 12.58 secs


Running hooks:  43% (53/121) zucchini_testdata                     
________ running 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --no_auth --num_threads=4 --bucket chromium-binary-patching/zucchini_testdata --recursive -d src/components/zucchini' in '/Users/dengkai01/chromium2'
3> Downloading src/components/zucchini/testdata/setup2.exe@12194273e8d509b6e81e4a6b63621081e1426028...
2> Downloading src/components/zucchini/testdata/chrome64_1.exe@4970ef6f342f6a0da9ae7a4ed462f93ef68f142c...
0> Downloading src/components/zucchini/testdata/chrome64_2.exe@c3a974589d50956a3c8c17572fee078b9276ad9b...
1> Downloading src/components/zucchini/testdata/setup1.exe@5d0e8fed8e9e091e184adb2e2e0e668def9cd2c5...
2> Downloading src/components/zucchini/fuzzers/testdata/new_eventlog_provider.dll@89ce67035d2d2dae33cb2d98d4762e955b93df95...
0> Downloading src/components/zucchini/fuzzers/testdata/old_eventlog_provider.dll@c80fdce994ba043956e192f650d894555460ff9b...
Downloading 6 files took 17.635683 second(s)
Hook 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --no_auth --num_threads=4 --bucket chromium-binary-patching/zucchini_testdata --recursive -d src/components/zucchini' took 19.19 secs


Running hooks:  50% (61/121) mediapipe_integration_testdata  
________ running 'python3 src/content/test/gpu/gpu_tests/mediapipe_update.py' in '/Users/dengkai01/chromium2'
0> Downloading /Users/dengkai01/chromium2/src/content/test/data/gpu/mediapipe_zip/mediapipe_chromium_tests.zip@1d6ca505c384ef8f5af14e7958f62d54ec126356...
Downloading 1 files took 15.248353 second(s)
Hook 'python3 src/content/test/gpu/gpu_tests/mediapipe_update.py' took 15.93 secs
Running hooks:  52% (64/121) subresource-filter-ruleset    
________ running 'python3 src/third_party/depot_tools/download_from_google_storage.py --no_resume --no_auth --bucket chromium-ads-detection -s src/third_party/subresource-filter-ruleset/data/UnindexedRules.sha1' in '/Users/dengkai01/chromium2'
0> Downloading src/third_party/subresource-filter-ruleset/data/UnindexedRules@a703507a694340bac1a78ce6e54dfbd641062da5...
Downloading 1 files took 7.820487 second(s)
Hook 'python3 src/testing/generate_location_tags.py --out src/testing/location_tags.json' took 27.00 secs
Running hooks: 100% (121/121), done.
```

思考: 这些hooks是在什么地方配置的？

