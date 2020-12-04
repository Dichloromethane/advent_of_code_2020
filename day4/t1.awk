#!/usr/bin/gawk -f

BEGIN {
  FS=":"
  keys[0] = "ecl"
  keys[1] = "hgt"
  keys[2] = "hcl"
  keys[3] = "byr"
  keys[4] = "iyr"
  keys[5] = "eyr"
  keys[6] = "cid"
  keys[7] = "pid"
}

function check(a) {
  for (i = 0; i < 8; i++) {
	k = keys[i]
	if ((k != "cid") && ! (k in a)) {
	  return 0;
	}
  }
  return 1;
}

/^$/ {
  if (check(a)) 
    num++
  delete a;
}

/^(ecl|hgt|hcl)/ {
  a[$1] = $2;
}

/^(byr|iyr|cid|eyr|pid)/ {
  a[$1] = int($2);
}

END {
  if (check(a)) 
    num++
  delete a;
  print num
}
