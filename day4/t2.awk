#!/usr/bin/gawk -f

BEGIN {
  FS=":"
  valid = 1
  num_entries = 0
}

/^$/ {
  if (num_entries == 7 && valid) 
    num++
  valid = 1
  num_entries = 0
}

/^byr:[0-9]{4}$/ {
  valid = valid && (1920 <= $2 && $2 <= 2002)
  num_entries++
}

/^iyr:[0-9]{4}$/ {
  valid = valid && (2010 <= $2 && $2 <= 2020)
  num_entries++
}

/^eyr:[0-9]{4}$/ {
  valid = valid && (2020 <= $2 && $2 <= 2030)
  num_entries++
}

/^hgt/ && match($2, /(.*)cm/, m) {
  valid = valid && (150 <= m[1] && m[1] <= 193)
  num_entries++
  next;
}
/^hgt/ && match($2, /(.*)in/, m) {
  valid = valid && (59 <= m[1] && m[1] <= 76)
  num_entries++
  next;
}
/hgt/ {
  valid = 0
}

/^hcl:#[0-9a-f]{6}$/ {
  num_entries++
  next;
}
/^hcl/ {
  valid = 0;
}

/^ecl:(amb|blu|brn|gry|grn|hzl|oth)/ {
  num_entries++
  next;
}
/^ecl/ {
  valid=0;
}

/^pid:[0-9]{9}$/ {
  num_entries++
  next
}
/^pid/ {
  valid = 0
}

END {
  if (num_entries == 7 && valid) 
    num++
  print num
}
