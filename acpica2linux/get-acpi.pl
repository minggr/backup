#
# get-acpi
#
# Checkout a version of ACPICA, including all source, headers, and
# makefiles. The parameter, if given, specifies the GIT tag to be checked out.
#
# requires GIT
#
# modify githome as necessary.
#

$githome = "/work/git/acpica";
$gitroot = "acpica";

$version;

# Get optional parameter and use as the GIT tag

if ($ARGV[0])
{
    $version = $ARGV[0];
}

# Checkout entire source and linux makefiles

system("echo Getting version $version of ACPICA");
system("rm -rf $gitroot; git clone $githome");
