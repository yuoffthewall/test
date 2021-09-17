set number
set ai
set background=dark
set cursorline
set hls
set tabstop=4
"set expandtab
set shiftwidth=4
set mouse=a

filetype indent on
colo torte

" key mapping
inoremap ( ()<Esc>i
inoremap {<CR> {<CR>}<Esc>ko
inoremap {{ {}<ESC>i
vnoremap <C-c> "+y
