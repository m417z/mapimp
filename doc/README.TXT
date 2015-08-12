MAPIMP plugin version 1.0
by takerZ
tPORt, 2009-2012

• What is this?
This is an OllyDbg plugin which will help you to import
map files exported by IDA, Dede, IDR, Microsoft and Borland
linkers.

• Why?
There are many plugins using which you can perform similar
actions, but mapimp:
- Recognizes debugged file segments and applies names correctly
- Has an option to overwrite or skip names that intersect already defined
- Has a filter option which gives you great name demangling potential
- Works fast (but who cares nowadays, right?)

• How to install?
Place plugin binary to plugins folder of the debugger you are using. See
INDEX file to find the corresponding one.

• How to use?
"Import map" menu item guides you to the file pick dialog. Pick the map
file you want to import and be happy. Just make sure your debugging session
is active. 
"Options" menu item guides you to the options dialog (thank you, captain
Obvious).

- Import objects
There are two objects you can import: comments and labels. Nothing
interesting here.

- Collisions
Check "Skip if collision" to protect already defined names from being
overwritten.
Check "Overwrite" otherwise.

- Read segments from
Check "File, memory on fail" to read segments virtual addresses from a PE
file adequate to your currently viewed module. If something goes wrong this
information will be read from memory.
Check "Memory, assume base on fail" to avoid reading PE file and to get this
information directly from memory. If this one fails mapimp will take in
assumption that there are 4 segments, that all have zero offset from the
region base. This enables you to import map files to executable regions, that
have no module on your physical drive.
Let's assume that you have a malware, that allocates an executable region and
writes a library to it. Then it passes control to some initialization
function, which fixes import address table, relocs and so on. You decide to
dump the region and analyze it in IDA. Now you have some symbolic data you
want to import to your debugging session. The problem is that there is no
library on your drive to read segments information from it. So reading this
information directly from the allocated region saves your ass. Shellcodes
have no segments at all, so assuming [base;base+size) as the only segment
still allows you to import your symbols correctly. On the other hand some
executable modules intentionally damage their headers to resist dumping and
so on. In this case you would rather read section list from file. Suming it
up: the first mode is for modules, that have an executable file, the second
one is for all others. Clear?

- If map file found
When new session starts mapimp searches for
DEBUGGEE_FILE_PATH\DEBUGGEE_FILE_NAME.MAP and proposes to load it right away.
Of course not everyone want to see the annoing message so the feature is
optional.

- Filter
The main idea is to apply series of masks to every single name loaded. Mask
syntax goes as follows:

/[i]key[replacement/]regex

Key "c" cuts the substring that matched regular expression.

Key "s" skips the name if regular expression succeeds. It may be useful if you
want to skip some dummy or incorrect names.

Key "r" replaces the substring that matched regular expression with your own.
Slash character separates the replacement from the regular expression. You can
also use "%" character to insert matched substring to the replacement. There
can be only one insert, so multiple percents lead to the fact that only the
first occurence will be recognized as a pseudo variable, others will be
ignored. To make slash or percent one of the characters of your replacement
string cover them with a backslash ("\/", "\%" and "\\" for backslash itself).

Key prefix "i" tells regular expression compiler to ignore character case when
regex will be executed.

Remember that if the name met mask condition it will be changed, then the
second mask is applied to the changed name and so on. The order of mask
applying is adequate to the order of appearance in the list. I also
recommend you to refer to the SAMPLES file.

As about regular expressions, the plugin uses PCRE library which syntax is
compatible with perl regular expressions. Check http://www.pcre.org for docs
and sources.

For adding, editing and other actions you can perform with masks use mask
manager's context menu.

Before being processed by mask filter names may be demangled. Microsoft and
Borland mangling schemes are supported and only names are undecorated. If map
file contains already demangled names the feature is ignored. Check "Demangle
names" to enable.

• Keyboard shortcuts

- From OllyDbg main window
Ctrl + Shift + I    Import map file
Ctrl + I            Options

- From mapimp options window
Ctrl + A       Add new mask
Ctrl + I       Insert new mask
Ctrl + E       Edit mask
Ctrl + S       Save mask list
Ctrl + L       Load mask list
Ctrl + Up      Move mask upwards
Ctrl + Down    Move mask downwards
Ctrl + PgUp    Move mask to the beginning
Ctrl + PgDown  Move mask to the end
Enter          Apply
Esc            Cancel

Also window control accelerator is availible. Use Alt + underscored letter to
interact.

• How to compile?
To compile the plugin by yourself you need PCRE library sources and your
debugger's plugin SDK. Currently supported debuggers are: OllyDbg 1.10 and
ImmunityDbg 1.85. To change the target debugger modify the appropriate switch
in mapimp.c by uncommenting it and commenting the others. For how to compile
PCRE library on win32 refer to the document called "NON-UNIX-USE" from the
source package. "BUILDING PCRE ON WINDOWS WITH CMAKE" paragraph contains
step-by-step guide up to what button you should press next. When you get all
the libraries pile them together, create a project using your favourite IDE
(I've used MSVC 8), configure the compiler for best performance and press
build button. You may have to modify import library names, so refer to
mapimp.h or solve it somehow on your own. A lot has been said about Immunity
Debugger PDK import library problem. There are some solutions floating on the
net, so I won't tell a thing here. By the way mapimp now resides on
googlecode, so you can take an import libraries of Immunity and static PCRE
library I've used from there.

• OllyDebugger 2?
Just wanted to place a little comment about OllyDbg2. I personally don't see
any strong reason for making mapimp support it, because it brings more new
bugs than any important improvements. Time does not stand still and I'm afraid
by the time it's codebase will become mature enough to solve most of problems
it will turn out to be morally obsolete. No offense, Oleh, you've done a great
job, but there is safety in numbers, just the way it is.
Anyway if working in OllyDbg2 is crucial for you, you may want to try this
port:
https://github.com/pezcode/mapimp


• Contacts
tracker: http://code.google.com/p/mapimp/issues/list
email:   push.ret@gmail.com