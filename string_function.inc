
macro strlen? str?*, retstr?*
    _assert_string (str)
    retstr = (lengthof (str))
end macro

macro strcat? str1?*, str2?*, retstr?*
    _assert_string (str1), (str2)
    retstr = (string (((str2) shl ((lengthof (str1)) * $08)) or (str1)))
end macro

macro strcmp? str1?*, str2?*, retstr?*
    local max_size?, ch1?, ch2?
    _assert_string (str1), (str2)
    max_size = (lengthof (str1))
    if (max_size < (lengthof (str2)))
        max_size = (lengthof (str2))
    end if
    retstr = $00
    repeat max_size
        ch1 = (((str1) shr ((% - $01) * $08)) and $FF)
        ch2 = (((str2) shr ((% - $01) * $08)) and $FF)
        if ~ (ch1 eq ch2)
            retstr = (ch1 - ch2)
            break
        end if
    end repeat
end macro

macro strin? str1?*, str2?*, retidx?*
    local str2_mask
    _assert_string (str1), (str2)
    str2_mask = ($01 shl (lengthof (str2) * $08)) - $01
    retidx = $00
    if ((lengthof (str1)) >= (lengthof (str2)))
        local retcmp
        repeat (((lengthof (str1)) - (lengthof (str2))) + $01)
            strcmp (string (((str1) shr ((% - $01) * $08)) and str2_mask)), (str2), retcmp
            if (~(retcmp))
                retidx = %
                break
            end if
        end repeat
    end if
end macro

macro strrin? str1?*, str2?*, retidx?*
    local last_index, skip_str
    _assert_string (str1), (str2)
    retidx = $00
    last_index = $00
    skip_str = (str1)
    while ($01)
        retidx = retidx + last_index
        skip_str = string ((skip_str) shr ($08 * last_index))
        strin skip_str, str2, last_index
        if (~(last_index))
            break
        end if
    end while
end macro

;; TODO HANDLE ERROR ;;
macro strsub? str?*, pos?*, len?*, retstr?*
    local len_mask, is_error
    _assert_string (str)
    _assert_numeric_unsigned (pos), (len)
    len_mask = (($01 shl (len * $08)) - $01)
    retstr = (string ((str shr ((pos - $01) * $08)) and len_mask))
end macro
;; TODO HANDLE ERROR ;;

macro _strcvtcase? str?*, retstr?*, head?*, tail?*
    local char
    retstr = $00
    repeat (lengthof (str))
        char = ((str shr ((% - $01) * $08)) and $FF)
        if ((char >= head) & (char <= tail))
            char = (char xor $20)
        end if
        retstr = ((retstr shl $08) or (char))
    end repeat
    retstr = (retstr bswap (lengthof (string (retstr))))
end macro

iterate <func,head,tail>, strupr,'a','z',\
                          strlwr,'A','Z'
    macro func? str?*, retstr?*
        _assert_string (str)
        _strcvtcase str, retstr, head, tail
    end macro
end iterate

macro strrep? str?*, rep?*, wth?*, retstr?*
    local reppos, lenrep, lenwth, temp1, temp2
    lenrep = (lengthof (rep))
    lenwth = (lengthof (wth))
    temp1  = str
    retstr   = $00
    while ($01)
        strin temp1, rep, reppos
        if ~ (reppos)
            break
        end if
        temp2 = (temp1 shr ((reppos - $01 + lenrep)  * $08))
        temp1 = (temp1 and (($01 shl ((reppos - $01) * $08)) - $01))
        strcat string (temp1), wth, temp1
        strcat string (retstr), temp1, retstr
        temp1 = string (temp2)
    end while
    strcat string (retstr), temp1, retstr
end macro
