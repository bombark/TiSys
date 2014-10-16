#!/bin/bash

proj_url=$(tisys.ctx Project);

src_url=$(project.info | tiobj.read source);
src_url="$proj_url/$src_url";

build_url=$(project.info | tiobj.read source);
build_url="$proj_url/$build_url";

mkdir -p $build_url;



cd $src_url; make;

