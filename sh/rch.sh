#!/bin/bash

input_file="Docs/Assignment0.html"
replacement_file="html/common-head.html"
output_file="temp.html"

replacement=$(cat "$replacement_file" | sed ':a;N;$!ba;s/\n/\\n/g')
escaped_replacement=$(printf '%s\n' "$replacement" | sed 's/[&/\]/\\&/g')

awk -v replacement="$escaped_replacement" '
/<div class="common-head">/ {inside=1; print $0; gsub(/\\n/, "\n", replacement); print replacement; next}
/<\/div>/ {if (inside) {inside=0; next}}
!inside {print}' "$input_file" > "$output_file"

# mv "$output_file" "$input_file"
