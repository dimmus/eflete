<h1 align="center">EFLETE (EFL Edge Theme Editor)</h1>

<p align="center">
  <img src="./data/images/eflete.svg" width="300" height="300" border="0"></>
</p>

<br/>
<p align="center">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/dimmus/eflete?style=for-the-badge">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/w/dimmus/eflete?style=for-the-badge">
  <img alt="Maintenance" src="https://img.shields.io/maintenance/yes/2023?style=for-the-badge">
  <a href="https://scan.coverity.com/projects/dimmus-eflete">
    <img alt="Coverity Scan Build Status" src="https://img.shields.io/coverity/scan/26855.svg?style=for-the-badge"/>
  </a>
</p>
<br/>

<p align="center">
  <i>Work in progress! Limited functionality.</i>
</p>

Efl Edje Theme Editor is a rich graphical tool for Enlightenment Foundation Library (EFL) that provides UI for edc markup. It is aimed at simplifying the creation and editing of widget styles. In addition, this is a great example of a full-fledged practical use of the amazing functionality of the EFL.

An EDC (Edje data collection) file is a text file that contains the code describing the position, size, and other parameters of graphical elements that compose the visual aspect of your EFL application. In addition to graphical elements, it can also handle sounds. EDC is a description language where the objects of an interface are described using a text description. An EDC file needs to be compiled into a `.edj` file using the Edje library tools. After compiling, the `.edj` file can be used as a theme file by a native EFL-based application.

No one, for sure, wants to learn EDC syntax. Eflete was designed with the idea that its target users are not just programmers but also designers. Besides styles, Eflete is also able to create and edit custom layouts. This is a useful feature for developers who create application layouts.

## Requirements

   * efl 1.26.99 (git)

Please note that some features may not quite function correctly or completely!

## Compiling

Once you have met the requirements, compiling and installing is simple:
```sh
autoreconf -i
./configure
make
sudo make install
```
If you want a really debuggable piece of code where optimizations mess with little to nothing at all, use:
```sh
./configure CFLAGS="-O -g -ffast-math -march=native -ggdb3"
```
Note that if you wish to compile for older architectures than your current system, please look up the compiler documentation and replace ''-march=native'' with something appropriate.

Use the --prefix option on the configure step to install the program to a specific directory:
```sh
./configure --prefix=/full/path/to/some/dir
```
You will want to ensure the default prefix `/usr/local` is available to build tools. If you know what you are doing, you can change the prefix, but this assumes you do not and the above prefix is used. These environment variables are used during the build, so you may want to make them more permanent.
```sh
export PATH=/usr/local/bin:"$PATH"
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:"$PKG_CONFIG_PATH"
export LD_LIBRARY_PATH=/usr/local/lib:"$LD_LIBRARY_PATH"
```
## Applying themes

To apply a theme to elementary, you should place the .edj-file in the elementary themes directory (default: `/usr/local/share/elementary/themes)`. Note: do not forget to add read permissions to the copied file. After that, the theme will be available for selection from `elementary_config`.

## Bash autocompletion

To enable bash autocompletion for eflete, copy `eflete_autocompletion.sh` file (you can find it in `scripts` folder) to your
bash completion directory or add the following line to your ~/.bashrc:
`source /YOUR/PATH/eflete_autocompletion.sh`

## Translations
You can help translate Eflete into your language. Eflete is using
[Weblate](https://weblate.org/) for translation management.

Go to the [Eflete localization project](https://hosted.weblate.org/projects/eflete/eflete/)
to start translating. If the language you want to translate into is not already
available, you [can add it here](https://hosted.weblate.org/new-lang/eflete/eflete/).

