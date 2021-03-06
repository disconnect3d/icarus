#!/usr/bin/env perl
use strict;
use warnings;
use v5.10;

my $path = "./icarus transform example/simple.ic";

my $output = `$path`;
my $exit_status = $?;
my $expected = <<EOF;
type Foo
    a::Signed
    b::String
end
fn maybe_add_one(b::Bool, i::Signed) -> Signed
    if b
        let _l0::Signed = 1s
        let _t0::Signed = plus(i, _l0)
        return _t0
    else
        return i
    end
end
fn main() -> Unit
    let _l0::Bool = True
    let _l1::Signed = 1s
    let _t0::Signed = maybe_add_one(_l0, _l1)
    let _l2::String = "hello"
    let f::Foo = Foo(_t0, _l2)
    println(f)
    let x::Signed = 6s
    let _l3::Signed = 14s
    let _t1::Bool = greaterthan(x, _l3)
    if _t1
        let _l4::String = "x > 14"
        println(_l4)
    else
        let _l5::String = "x <= 14"
        println(_l5)
    end
end
EOF

if( $exit_status != 0 || $output ne $expected ){
    say "Output was not as expected";
    say "=======\nExpected:\n$expected";
    say "=======\nGot:\n$output";
    say "=======\n";
    if( $exit_status != 0 ){
        die "exit_status was '$exit_status', expected 0";
    } else {
        die "Output not as expected";
    }
}

say "test_transform_simple successs";

