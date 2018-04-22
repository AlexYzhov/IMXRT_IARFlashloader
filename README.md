# IMXRT_IARFlashloader

## 本工程参照IAR自带的flashloader样例工程，结合KSDK2.3的部分驱动代码，针对IMX.RT1052(未来还会添加更多IMX.RT系列的芯片)实现了多款nor flash的下载算法。

工程的基本结构如下:

"/IMXRT_IARFlashloader/Devices/": 不同的flash型号的驱动文件，和NXP面向IAR flashloader框架提供的Device级抽象(lut.h, device.h)

"/IMXRT_IARFlashloader/Drivers/": 基于KSDK2.3的驱动代码，无需修改任何内容

"/IMXRT_IARFlashloader/Flashloader/": IAR提供的Flashloader框架代码，无需修改任何内容。编写flashloader的关键在于需要自行实现FlashInit, FlashWrite和FlashErase三个函数的实现。

"/IMXRT_IARFlashloader/Framework/": 该路径下有多个文件夹，是针对市面上目前存在的多款RT1052开发板建立的flashloader。使用对应文件夹下的.board, .flash和.out文件当作代码工程的flashloader，可以实现将代码下载至片外flash。