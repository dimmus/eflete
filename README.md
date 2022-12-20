<h1 align="center">EFLETE (EFL Edge Theme Editor)</h1>

<p align="center">
  <img src="./data/images/eflete.svg" width="300" height="300" border="0"></>
</p>

<br/>
<p align="center">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/dimmus/eflete?style=for-the-badge">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/w/dimmus/eflete?style=for-the-badge">
  <img alt="Maintenance" src="https://img.shields.io/maintenance/yes/2022?style=for-the-badge">
</p>
<br/>

<p align="center">
  <i>Work in progress! Limited functionality.</i>
</p>

Efl Edje Theme Editor is a rich graphical tool, that provide UI for edc markup.
It is aimed to simplify creation and editing widgets styles. No one, for sure,
want to learn EDC syntax. Eflete was designed with mind, that target users are
not just programmers but also designers. Besides styles Eflete is also able to
create and edit custom layouts. This is useful feature for developers which
creates applications layouts.

```
For comfortable work with Eflete your monitor should have the HD (1366x768) resolution. If resolution is less not all UI componenets is displayed.
```

## Requirements

   * efl (>= 1.23.3 && < 1.26.99)

Please note that some features may not quite function correctly or
completely!

## Compiling

Once you have met requirements, compiling and installing is simple:
```sh
./autogen.sh
make
sudo make install
```
If you want a really debuggable piece of code where optimizations mess with little to nothing at all use:
```sh
./configure CFLAGS="-O -g -ffast-math -march=native -ggdb3"
```
Note that if you wish to compile for older architectures than your current system, please look up the compiler documentation and replace ''-march=native'' with something appropriate.

To install program to specified directory use --prefix option on configure step:
```sh
./autogen.sh --prefix=/full/path/to/some/dir
```
You will want to ensure the default prefix `/usr/local` is available to build tools. If you know what you are doing, you can change the prefix, but this here shall assume you do not, and the above prefix is used. These environment variables are used during build, so you may want to make them more permanent.
```sh
export PATH=/usr/local/bin:"$PATH"
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:"$PKG_CONFIG_PATH"
export LD_LIBRARY_PATH=/usr/local/lib:"$LD_LIBRARY_PATH"
```
## Applying themes

To apply created theme to elementary you should place exported .edj-file to elementary themes dir (default: `/usr/local/share/elementary/themes)`. Note do not forget to add read permissions to copied file. After that theme will be available for selection from `elementary_config`.

## Bash autocompletion

To enable bash autocompletion for eflete copy eflete_autocompletion.sh to your
bash completion directory or add followig line to your .bashrc:
`source /YOUR/PATH/eflete_autocompletion.sh`
