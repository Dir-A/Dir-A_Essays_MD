**Что делать с файлом Asprvm8s.bin**

Скопируйте файл Asprvm8s.bin в любую папку, а затем откройте в Блокноте или в любом текстовом редакторе скрипт. В поиске найдите строчку, где используется название файла Asprvm8s.bin и измените там путь до файла Asprvm8s.bin.
Всего в скрипте два таких места, это:

lab78_1:
log VMcodeloc
lm VMcodeloc, 4000, "d:\Asprvm8s.bin"         ---> измените на настоящий путь

И другое место ,это:

FillSCP2:
//alloc 10000
//mov VMcodeloc, $RESULT
//log VMcodeloc
//lm VMcodeloc, 4000, "d:\Asprvm8s.bin"         ---> измените на настоящий путь

Мой совет, лучше кидать файл Asprvm8s.bin в корень диска, например c:\Asprvm8s.bin или d:\Asprvm8s.bin