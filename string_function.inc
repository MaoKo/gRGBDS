
struc strlen? str?*
    _assert_string (str)
    . = (lengthof (str))
end struc

struc strcat? str1?*, str2?*
    _assert_string (str1), (str2)
    . = (string (((str2) shl ((lengthof (str1)) * $08)) or (str1)))
end struc

struc strcmp? str1?*, str2?*
    local max_size?, ch1?, ch2?
    _assert_string (str1), (str2)
    max_size = (lengthof (str1))
    if (max_size < (lengthof (str2)))
        max_size = (lengthof (str2))
    end if
    . = $00
    repeat max_size
        ch1 = (((str1) shr ((% - $01) * $08)) and $FF)
        ch2 = (((str2) shr ((% - $01) * $08)) and $FF)
        if (~(ch1 eq ch2))
            . = (ch1 - ch2)
            break
        end if
    end repeat
end struc

struc strin? str1?*, str2?*
    local str2_mask
    _assert_string (str1), (str2)
    str2_mask = ($01 shl (lengthof (str2) * $08)) - $01
    . = $00
    if ((lengthof (str1)) >= (lengthof (str2)))
        local retcmp
        repeat (((lengthof (str1)) - (lengthof (str2))) + $01)
            retcmp strcmp (string (((str1) shr (((%) - $01) * $08)) and str2_mask)), (str2)
            if (~(retcmp))
                . = (%)
                break
            end if
        end repeat
    end if
end struc

struc strrin? str1?*, str2?*
    local last_index, skip_str
    _assert_string (str1), (str2)
    . = $00
    last_index = $00
    skip_str = (str1)
    while ($01)
        . = . + last_index
        skip_str = string ((skip_str) shr ($08 * last_index))
        last_index strin skip_str, str2
        if (~(last_index))
            break
        end if
    end while
end struc

;; TODO HANDLE ERROR ;;
macro strsub? str?*, pos?*, len?*, retstr?*
    local len_mask, is_error
    _assert_string (str)
    _assert_numeric_unsigned (pos), (len)
    len_mask = (($01 shl (len * $08)) - $01)
    retstr = (string ((str shr ((pos - $01) * $08)) and len_mask))
end macro
;; TODO HANDLE ERROR ;;

struc _strcvtcase? str?*, head?*, tail?*
    local char
    . = $00
    repeat (lengthof (str))
        char = ((str shr ((% - $01) * $08)) and $FF)
        if ((char >= head) & (char <= tail))
            char = (char xor $20)
        end if
        . = ((. shl $08) or (char))
    end repeat
    . = (. bswap (lengthof (string (.))))
end struc

iterate <pred,head,tail>, strupr,'a','z',\
                          strlwr,'A','Z'
    struc pred? str?*
        _assert_string (str)
        . _strcvtcase str, head, tail
    end struc
end iterate

struc strrep? str?*, rep?*, wth?*
    local reppos, lenrep, lenwth, temp1, temp2
    lenrep = (lengthof (rep))
    lenwth = (lengthof (wth))
    temp1  = str
    . = $00
    while ($01)
        reppos strin temp1, rep
        if (~(reppos))
            break
        end if
        temp2 = (temp1 shr ((reppos - $01 + lenrep)  * $08))
        temp1 = (temp1 and (($01 shl ((reppos - $01) * $08)) - $01))
        temp1 strcat string (temp1), wth
        .     strcat string (.), temp1
        temp1 = string (temp2)
    end while
    . strcat string (.), temp1
end struc
