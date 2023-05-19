#!/bin/bash

make run

./assembler \
	./examples/mmnexample_1 \
	 ./examples/mmnexample \
	  ./examples/macro \
	   ./examples/comments \
	   ./examples/simple \
	   ./examples/one_command

files=("one_command" "simple" "comments" "mmnexample" "mmnexample_1")
for file in "${files[@]}"; do
    echo -e "[--------- testing $file ---]"
    if cmp -s "$file.am" "$file.am.expected"; then
        printf 'Failed macro expand test for "%s" \n' "$file" 

    else
       printf 'Passed macro expand test for "%s" \n' "$file" 

    fi

    if cmp -s "$file.ob" "$file.ob.expected"; then
        printf 'Failed macro expand test for "%s" \n' "$file" 
    else
        printf 'Passed macro expand test for "%s" \n' "$file" 

    fi

    echo ""

done
