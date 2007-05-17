#!/bin/sh

kaneton_dir=~/kaneton
figures_dir=$kaneton_dir/view/lecture/kernels/all/figures

[ -d $figures_dir ] || mkdir -p $figures_dir

for dir in `ls  -I "all" $kaneton_dir/view/lecture/kernels`; do
    [ -d $kaneton_dir/view/lecture/kernels/$dir/figures ] || continue

    for fig in `ls -I "*.fig" $kaneton_dir/view/lecture/kernels/$dir/figures/`; do
	ln -s $kaneton_dir/view/lecture/kernels/$dir/figures/$fig $figures_dir/$fig
    done

done
