## Pre-requis :
- installer le gestionnaire de paquets Brew
	https://brew.sh/
- installer Git a l'aide de Brew
	Dans un terminal : `brew update && brew install git`
- installer VirtualBox
	https://www.virtualbox.org/wiki/Downloads
- installer Docker-Machine a l'aide de Brew
	Dans un terminal : `brew update && brew install docker-machine`

## Puis, dans un terminal :
- creer une machine virtuelle utilisant le driver VirtualBox
	`docker-machine create --driver virtualbox my_virtual-machine`
- assigner les variables d'environnement necessaires a l'utilisation de Docker dans le terminal courant
	`eval "$(docker-machine env my_virtual_machine)"`
- telecharger une image debian depuis DockerHub
	`docker pull debian:stretch`
- creer un Dockerfile ressemblant a :
```
FROM debian

# (lien suivant provenant de <https://github.com/ValeryBruniaux/md2htmlpdf/releases>)
ARG SRC_MD2HTMLPDF https://github.com/ValeryBruniaux/md2htmlpdf/archive/1.01.tar.gz
# (lien suivant provenant de <https://wkhtmltopdf.org/downloads.html>)
ARG SRC_WKHTMLTOPDF https://downloads.wkhtmltopdf.org/0.12/0.12.5/wkhtmltox_0.12.5-1.stretch_amd64.deb

RUN apt-get update && apt-get -y install tar gzip pandoc openssl fontconfig libssl-dev wget

RUN mkdir -p /MarkDown_to_PDF
RUN mkdir -p /MarkDown_to_PDF/workdir
RUN mkdir -p /MarkDown_to_PDF/sources
RUN mkdir -p /Markdown_to_PDF/sources/MD2HTMLPDF
RUN wget -O /MarkDown_to_PDF/sources/MD2HTMLPDF/src_md2htmlpdf.tar.gz SRC_MD2HTMLPDF
RUN mkdir -p /MarkDown_to_PDF/sources/WKHTMLTOPDF
RUM wget -O /MarkDown_to_PDF/sources/WKHTMLTOPDF/src_wkhtmltopdf.deb SRC_WKHTMLTOPDF

RUN tar -xvzf -C /MarkDown_to_PDF/sources/MD2HTMLPDF/ --strip-components=1 src_md2htmlpdf.tar.gz
RUN sh /MarkDown_to_PDF/sources/MD2HTMLPDF/install.sh
RUN dpkg -i /MarkDown_to_PDF/sources/WKHTMLTOPDF/src_wkhtmltopdf.deb
```
- generer une image Docker a partir du Dockerfile precedent
	`docker build --tag markdown_to_pdf ./`
- creer un script qui sera a lancer dans un terminal pour convertir un fichier MarkDown en fichier PDF, ressemblant a :
```
#!/bin/sh
echo 'Donner le nom (sans l'extension .md) du fichier a convertir en PDF :'
read FILENAME
if test ! -e "${FILENAME}.md" ; then
	echo 'Le fichier specifie n'existe pas sur votre bureau !'
else
	eval "$(docker-machine env my_virtual_machine)"
	docker run
```
