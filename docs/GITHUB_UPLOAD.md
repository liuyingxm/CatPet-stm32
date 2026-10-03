# 上传到GitHub

## 准备内容

上传目录为 `CatPet-stm32`。根目录应能直接看到：

- `README.md`
- `CatPet-stm32.uvprojx`
- `Start/`、`Library/`、`System/`、`Hardware/`、`User/`
- `docs/`
- `.gitignore`、`.gitattributes`、`THIRD_PARTY_NOTICES.md`

编译以后产生的 `Objects/`、`Listings/`、`DebugConfig/` 和本机的 `.uvoptx`、`.uvguix.*` 不需要上传。

## 网页上传

1. 登录GitHub，创建名为 **CatPet-stm32** 的仓库。
2. 进入仓库的文件上传页面。
3. 上传整理目录中的文件和源码文件夹。让README和Keil工程文件位于仓库根目录，避免再套一层 `CatPet-stm32` 文件夹。
4. 完成后提交，提交说明可填写 `Initial CatPet STM32 project`。
5. 查看仓库首页，确认README、三张预览图片和源码目录正常显示。

压缩包用于下载、传递或备份。网页上传源码时使用解压后的内容；仅上传ZIP文件不会把里面的源码展开到仓库中。

使用Git客户端上传时，根目录的 `.gitignore` 会自动排除编译产物；网页上传时，需要自行选择要上传的文件。
