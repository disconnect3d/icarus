#!/usr/bin/env perl
use strict;
use warnings;
use v5.10;
use IPC::Open3;

my $path = "bin/t/custom/test_backend_pancake_interpret_instructions";

die "Could not find '$path'\n" unless -e $path;

my $cases = [
  {
    input => "
      pushuint 176
      pushuint 7
      pushuint 27
      exit
      ",
    expected => "
      uint: 27
      uint: 7
      uint: 176
      "
  },
  {
    input => "
      label dummy
      pushuint 4
      pushuint 3
      pushuint 6
      pushuint 7
      call_builtin plus(Uint,Uint) 2
      call_builtin plus(Uint,Uint) 2
      exit
      ",
    expected => "
      uint: 16
      uint: 4
      "
  },
  {
    input => "
      pushuint 17
      call_builtin println(Uint) 1
      exit
      ",
    expected => "
      17
      "
  },
  {
    input => "
      pushint 3
      pushint 5
      call_builtin minus(Sint,Sint) 2
      exit
      ",
    expected => "
      sint: -2
      "
  },
  {
    input => "
      pushuint 3
      pushuint 5
      call_builtin minus(Uint,Uint) 2
      exit
      ",
    expected => "
      uint: 0
      "
  },
];

# whitespace sensitivity sucks
sub cleanup {
  my $str = shift // die;
  die if @_;

  # strip any leading whitespace on each line
  $str =~ s/^\s*//mg;

  # strip any trailing whitespace on each line
  $str =~ s/\s*$//mg;

  # strip any blank lines
  $str =~ s/^\s*$//mg;

  return $str;
}

sub run {
  my $input = shift // die;
  my $expected = shift // die;
  die if @_;

  my $pid = open3(my $write, my $read, my $error, "$path") or die "Failed to open $path";
  print $write $input;
  close $write;
  waitpid($pid, 0);
  my $output = "";
  {
      local $/ = undef;
      $output = <$read>;
  }
  $output = cleanup $output;

  if( $output ne $expected ){
      say "Output was not as expected";
      say "=======\nExpected:\n$expected";
      say "=======\nGot:\n$output";
      say "=======\n";
      die "Output not as expected";
  }

  say "=======\nGot correct output:\n$output";
}

for my $case (@$cases) {
  my $input = cleanup $case->{input};
  my $expected = cleanup $case->{expected};
  run $input, $expected;
}

say "test_backend_pancake_interpret_simple successs";
