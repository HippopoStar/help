## Pre-requis :
- installer le gestionnaire de paquets Brew  
	https://brew.sh/
- installer Docker ainsi que Docker-Machine a l'aide de Brew  
	Dans un terminal : `brew update && brew install docker docker-machine`
- installer VirtualBox  
	https://www.virtualbox.org/wiki/Downloads

### Si votre repertoire HOME est situe sur un SSD et que vous possedez par ailleurs un HDD, il est conseille de deplacer le repertoire de travail de Docker sur votre HDD. Pour cela, effectuez les commandes suivantes :

	mv $HOME/.docker <emplacement_de_destination>
	ln -s <emplacement_de_destination>/.docker $HOME/


## Puis, dans un terminal :
- creer une machine virtuelle utilisant le driver VirtualBox  
	`docker-machine create --driver virtualbox my-virtual-machine`
- assigner les variables d'environnement necessaires a l'utilisation de Docker dans le terminal courant  
	`eval "$(docker-machine env my-virtual-machine)"`
- telecharger une image debian depuis DockerHub  
	`docker pull debian:stretch`
- creer un Dockerfile ressemblant a :
>```
>FROM debian:stretch
>
># voir GNU Linux Pratique n109
># (lien suivant provenant de <https://github.com/ValeryBruniaux/md2htmlpdf/releases>)
>ARG SRC_MD2HTMLPDF=https://github.com/ValeryBruniaux/md2htmlpdf/archive/1.01.tar.gz
># (lien suivant provenant de <https://wkhtmltopdf.org/downloads.html>)
>ARG SRC_WKHTMLTOPDF=https://downloads.wkhtmltopdf.org/0.12/0.12.5/wkhtmltox_0.12.5-1.stretch_amd64.deb
>
>RUN apt-get update && apt-get -y install tar gzip pandoc openssl fontconfig libssl-dev wget vim sudo libjpeg62-turbo libx11-6 libxcb1 libxext6 libxrender1 xfonts-75dpi xfonts-base
>
>RUN mkdir -p /MarkDown_to_PDF
>RUN mkdir -p /MarkDown_to_PDF/workdir
>RUN mkdir -p /MarkDown_to_PDF/sources
>
>RUN mkdir -p /root/.config
>RUN mkdir -p /Markdown_to_PDF/sources/MD2HTMLPDF
>WORKDIR /MarkDown_to_PDF/sources/MD2HTMLPDF/
>RUN wget -O src_md2htmlpdf.tar.gz $SRC_MD2HTMLPDF
>RUN tar -xvzf src_md2htmlpdf.tar.gz --strip-components=1
>RUN sh install.sh
>
># RUN mkdir -p /MarkDown_to_PDF/sources/WKHTMLTOPDF
># WORKDIR /MarkDown_to_PDF/sources/WKHTMLTOPDF/
># RUN wget -O src_wkhtmltopdf.deb $SRC_WKHTMLTOPDF
># RUN dpkg -i src_wkhtmltopdf.deb
>
>WORKDIR /MarkDown_to_PDF/workdir/
>
>ENTRYPOINT [ "/bin/bash" ]
>```
- generer une image Docker a partir du Dockerfile precedent  
	`docker build --tag markdown_to_pdf_image ./`
- creer un script qui sera a lancer dans un terminal pour convertir un fichier MarkDown en fichier PDF, ressemblant a :
>```
>#!/bin/sh
>echo 'Donner le nom (sans l extension .md) du fichier a convertir en PDF :'
>read FILENAME
>if test ! -e "$HOME/Desktop/${FILENAME}.md" ; then
>	echo 'Le fichier specifie n existe pas sur votre bureau !'
>else
>	eval "$(docker-machine env my-virtual-machine)"
>	docker exec markdown_to_pdf_container md2htmlpdf "${FILENAME}.md"
>	docker run --name markdown_to_pdf_container --volume $HOME/Desktop/:/MarkDown_to_PDF/workdir/ --rm markdown_to_pdf_image md2htmlpdf "${FILENAME}"
>fi
>```
- rendre le script precedent executable  
	`chmod u+x <votre_script>`
