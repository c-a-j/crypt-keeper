#!/usr/bin/env bash

set -euo pipefail

reset=$'\033[0m'
bold=$'\033[1m'
dim=$'\033[2m'
underline=$'\033[4m'
reverse=$'\033[7m'

print_sample() {
  local label=$1
  local seq=$2
  printf '%-18s %sSample text 0123 !@#%s\n' "$label" "$seq" "$reset"
}

print_rgb_sample() {
  local label=$1
  local r=$2
  local g=$3
  local b=$4
  local seq
  seq=$(printf '\033[38;2;%s;%s;%sm' "$r" "$g" "$b")
  printf '%-18s %sRGB(%3s,%3s,%3s)%s\n' "$label" "$seq" "$r" "$g" "$b" "$reset"
}

print_header() {
  local title=$1
  printf '\n%s%s%s\n' "$bold" "$title" "$reset"
}

print_header "Project Palette"
print_sample "Gray"       $'\033[38;2;146;131;116m'
print_sample "Red"        $'\033[38;2;251;73;52m'
print_sample "Green"      $'\033[38;2;184;187;38m'
print_sample "Yellow"     $'\033[38;2;250;189;47m'
print_sample "Blue"       $'\033[38;2;131;165;152m'
print_sample "BoldBlue"   $'\033[1;34m'
print_sample "Aqua"       $'\033[38;2;142;192;124m'
print_sample "Orange"     $'\033[38;2;254;128;25m'
print_sample "Purple"     $'\033[38;2;211;134;155m'
print_sample "BoldPurple" $'\033[1;38;2;211;134;155m'

print_header "Standard ANSI Blue Variants"
print_sample "Blue"           $'\033[34m'
print_sample "Bold Blue"      $'\033[1;34m'
print_sample "Underline Blue" $'\033[4;34m'
print_sample "Reverse Blue"   $'\033[7;34m'
print_sample "Bold+Under"     $'\033[1;4;34m'

print_header "Candidate Truecolor Blues"
print_rgb_sample "Muted teal"     131 165 152
print_rgb_sample "Bright blue"     83 155 245
print_rgb_sample "Deeper blue"     69 133 245
print_rgb_sample "Cool blue"       80 120 255
print_rgb_sample "Saturated blue"  64  96 255
print_rgb_sample "Pass-ish blue"   70 130 240

print_header "Modifiers On One Blue"
print_sample "Normal"    $'\033[38;2;83;155;245m'
print_sample "Bold"      $'\033[1;38;2;83;155;245m'
print_sample "Dim"       $'\033[2;38;2;83;155;245m'
print_sample "Underline" $'\033[4;38;2;83;155;245m'
print_sample "Reverse"   $'\033[7;38;2;83;155;245m'

print_header "256-Color Blue Ramp"
for code in 17 18 19 20 21 25 26 27 31 32 33 39 45 69 75 81 117; do
  printf 'color %-9s \033[38;5;%smSample text 0123 !@#%s\n' "$code" "$code" "$reset"
done
