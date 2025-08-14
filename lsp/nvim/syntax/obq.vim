" Vim syntax file for O²L programming language
" Language: O²L
" Maintainer: O²L Development Team
" Latest Revision: 2024

if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword obqKeyword Object method property constructor
syn keyword obqKeyword import this return throw try catch finally
syn keyword obqKeyword if else while for break continue
syn keyword obqKeyword enum record protocol const
syn keyword obqKeyword true false null

" Types
syn keyword obqType Text Int Float Bool Char Double Long
syn keyword obqType List Map Set Optional Result

" Operators
syn match obqOperator "="
syn match obqOperator "+"
syn match obqOperator "-"
syn match obqOperator "\*"
syn match obqOperator "/"
syn match obqOperator "%"
syn match obqOperator "=="
syn match obqOperator "!="
syn match obqOperator "<"
syn match obqOperator ">"
syn match obqOperator "<="
syn match obqOperator ">="
syn match obqOperator "&&"
syn match obqOperator "||"
syn match obqOperator "!"
syn match obqOperator "\."
syn match obqOperator ":"
syn match obqOperator "->"

" Delimiters
syn match obqDelimiter "("
syn match obqDelimiter ")"
syn match obqDelimiter "{"
syn match obqDelimiter "}"
syn match obqDelimiter "\["
syn match obqDelimiter "\]"
syn match obqDelimiter ","
syn match obqDelimiter ";"

" Numbers
syn match obqNumber "\<\d\+\>"
syn match obqNumber "\<\d\+\.\d*\>"
syn match obqNumber "\<\d*\.\d\+\>"
syn match obqNumber "\<\d\+[fFdDlL]\>"
syn match obqNumber "\<\d\+\.\d*[fFdD]\>"

" Strings
syn region obqString start='"' end='"' contains=obqStringEscape
syn match obqStringEscape "\\." contained

" Characters
syn region obqChar start="'" end="'" contains=obqCharEscape
syn match obqCharEscape "\\." contained

" Comments
syn match obqComment "//.*$"
syn region obqComment start="/\*" end="\*/"

" Identifiers
syn match obqIdentifier "[a-zA-Z_][a-zA-Z0-9_]*"

" Annotations
syn match obqAnnotation "@[a-zA-Z_][a-zA-Z0-9_]*"

" Define the default highlighting
hi def link obqKeyword     Keyword
hi def link obqType        Type
hi def link obqOperator    Operator
hi def link obqDelimiter   Delimiter
hi def link obqNumber      Number
hi def link obqString      String
hi def link obqChar        Character
hi def link obqComment     Comment
hi def link obqIdentifier  Identifier
hi def link obqAnnotation  PreProc
hi def link obqStringEscape Special
hi def link obqCharEscape   Special

let b:current_syntax = "obq"