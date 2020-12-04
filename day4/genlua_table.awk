#!/usr/bin/gawk -f

BEGIN {
  printf("return {[0] = {\n");
  FS=":"
}

/^$/ {
  printf("},\n[%d]={\n", ++i);
}

# {
#     print gensub(/([^:]*):(.*)/, "=\"\\1\"", "g", $i);
# }

/^(ecl|hgt|hcl)/ {
  printf("  %s=\"%s\",\n", $1, $2);
}

/^(byr|iyr|cid|eyr)/ {
  printf("  %s=%d,\n", $1, $2);
}

END {
  printf("}}\n");
}
