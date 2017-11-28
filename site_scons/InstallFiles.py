# File:         install.py
# Author:       Brian A. Vanderburg II
# Purpose:      An install builder to install subdirectories and files
# Copyright:    This file is placed in the public domain.
##############################################################################


# Requirements
##############################################################################
import os
import fnmatch

import SCons
import SCons.Node.FS
import SCons.Errors
from SCons.Script import *


# Install files code
##############################################################################

def _is_excluded(name, exclude):
    """
    Determine if the name is to be excluded based on the exclusion list
    """
    if not exclude:
        return False
    return any( (fnmatch.fnmatchcase(name, i) for i in exclude) )


def _is_globbed(name, glob):
    """
    Determine if the name is globbed based on the glob list
    """
    if not glob:
        return True
    return any( (fnmatch.fnmatchcase(name, i) for i in glob) )


def _get_files(env, source, exclude, glob, recursive, reldir=os.curdir):
    """
    Find files that match the criteria.

    source - absolute path to source file or directory (not a node)
    exclude - additional exclusion masks in addition to default
    glob - glob masks
    recursive - scan recursively or not

    Returns a list of 2-element tuples where the first element is
    relative to the source, the second is the absolute file name.
    """

    # Make sure it exists and is not a link
    if not os.path.exists(source):
        return []

    if os.path.islink(source):
        return []

    # Handle file directly
    if os.path.isfile(source):
        return [ (os.path.join(reldir, os.path.basename(source)), source) ]

    # Scan source files on disk
    if not os.path.isdir(source):
        return []

    results = []
    for entry in os.listdir(source):
        fullpath = os.path.join(source, entry)
        if os.path.islink(fullpath):
            continue

        # Excluded (both files and directories)
        if _is_excluded(entry, exclude):
            continue

        # File
        if os.path.isfile(fullpath):
            if _is_globbed(entry, glob):
                results.append( (os.path.join(reldir, entry), fullpath) )
        elif os.path.isdir(fullpath):
            if recursive:
                newrel = os.path.join(reldir, entry)
                results.extend(_get_files(env, fullpath, exclude, glob, recursive, newrel))

    return results


def _get_built_files(env, source, exclude, glob, recursive, reldir=os.curdir):
    """
    Find files that match the criteria.

    source - source file or directory node
    exclude - additional exclusion masks in addition to default
    glob - glob masks
    recursive - scan recursively or not

    Returns a list of 2-element tuples where the first element is
    relative to the source, the second is the absolute file name.
    """

    # Source
    source = source.disambiguate()

    # If a file, return it without scanning
    if isinstance(source, SCons.Node.FS.File):
        if source.is_derived():
            source = source.abspath
            return [ (os.path.join(reldir, os.path.basename(source)), source) ]
        else:
            return []

    if not isinstance(source, SCons.Node.FS.Dir):
        return []

    # Walk the children
    results = []

    for child in source.children():
        child = child.disambiguate()
        name = os.path.basename(child.abspath)

        # Ignore '.' and '..'
        if name == '.' or name == '..':
            continue

        # Exclude applies to files and directories
        if _is_excluded(name, exclude):
            continue

        if isinstance(child, SCons.Node.FS.File):
            if child.is_derived() and _is_globbed(name, glob):
                results.append( (os.path.join(reldir, name), child.abspath) )
        elif isinstance(child, SCons.Node.FS.Dir):
            if recursive:
                newrel = os.path.join(reldir, name)
                results.extend(_get_built_files(env, child, exclude, glob, recursive, newrel))

    return results


def _get_both(env, source, exclude, glob, recursive, scan):
    """
    Get both the built and source files that match the criteria.  Built
    files take priority over a source file of the same path and name.
    """

    src_nodes = []
    results = []

    # Get built files
    if scan == 0 or scan == 2:
        results.extend(_get_built_files(env, source, exclude, glob, recursive))
        for (relsrc, src) in results:
            node = env.File(src)
            src_nodes.append(node.srcnode())

    # Get source files
    if scan == 1 or scan == 2:
        files = _get_files(env, source.srcnode().abspath, exclude, glob, recursive)
        for (relsrc, src) in files:
            node = env.File(src)
            if not node in src_nodes:
                results.append( (relsrc, src) )

    return results
                

def InstallFiles(env, target, source, exclude=None, glob=None, recursive=True, scan=2):
    """
    InstallFiles pseudo-builder

    target - target directory to install to
    source - source file or directory to scan
    exclude - a list of patterns to exclude in files and directories
    glob - a list of patterns to include in files
    recursive - scan directories recursively
    scan - 0=scan built nodes, 1=scan source files, 2=both

    All argument except target and source should be used as keyword arguments
    """

    # Information
    if exclude:
        exclude = Flatten(exclude)
    else:
        exclude = []
    exclude.extend(env['INSTALLFILES_EXCLUDES'])

    if glob:
        glob = Flatten(glob)
    else:
        glob = []

    # Flatten source/target
    target = Flatten(target)
    source = Flatten(source)

    if len(target) != len(source):
        if len(target) == 1:
            # If only one target, assume it is for all the sources
            target = target * len(source)
        else:
            raise SCons.Errors.UserError('InstallFiles expects only one target directory or one for each source')

    # Scan
    files = []
    for (t, s) in zip(target, source):
        if not isinstance(t, SCons.Node.FS.Base):
            t = env.Dir(t)
        if not isinstance(s, SCons.Node.FS.Base):
            s = env.Entry(s)

        for (relsrc, src) in _get_both(env, s, exclude, glob, recursive, scan):
            dest = os.path.normpath(os.path.join(t.abspath, relsrc))
            files.append( (dest, src) )

    # Install
    results = []
    for (dest, src) in files:
        results.extend(env.InstallAs(dest, src))

    # Done
    return results


def InstallPackageAccum(env, name, target, source, exclude=None, glob=None, recursive=True, scan=2):
    """
    InstallPackageAccum accumulate files for a package name

    name - the name of the package of files
    target - relative target directory under the package directory, can be '.'
    source - source file or directory to scan
    exclude - a list of patterns to exclude in files and directories
    glob - a list of patterns to include in files
    recursive - scan directories recursively
    scan - 0=scan built nodes, 1=scan source files, 2=both

    All argument except target and source should be used as keyword arguments and
    target should NOT be nodes but strings such as '.'
    """
    
    # Information
    if exclude:
        exclude = Flatten(exclude)
    else:
        exclude = []
    exclude.extend(env['INSTALLFILES_EXCLUDES'])

    if glob:
        glob = Flatten(glob)
    else:
        glob = []

    # Flatten target/source
    target = Flatten(target)
    source = Flatten(source)

    if len(target) != len(source):
        if len(target) == 1:
            target = target * len(source)
        else:
            raise SCons.Errors.UserError('InstallPackageAccum expects only one target directory or one for each source')

    # Scan
    files = []
    for (t, s) in zip(target, source):
        t = env.subst(t)
        if not isinstance(s, SCons.Node.FS.Base):
            s = env.Entry(s)

        for (relsrc, src) in _get_both(env, s, exclude, glob, recursive, scan):
            dest = os.path.normpath(os.path.join(t, relsrc))
            files.append( (dest, src) )

    # Add package if needed
    packages = env['INSTALLFILES_PACKAGES']
    if not name in packages:
        packages[name] = []
    packages[name].extend(files)


def InstallPackage(env, target, name):
    """
    Install the files of a given package to a certain location

    target - the directory to install the package to
    name - the name of the package to install
    """

    # Flatten target/name
    target = Flatten(target)
    name = Flatten(name)

    if len(target) != len(name):
        if len(target) == 1:
            target = target * len(name)
        else:
            raise SCons.Errors.UserError('InstallPackage expects only one target directory or one for each package')

    # Install
    results = []
    packages = env['INSTALLFILES_PACKAGES']

    for (t, n) in zip(target, name):
        if not n in packages:
            raise SCons.Errors.UserError('InstallPackage package name does not exist: ' + n)

        for (relsrc, src) in packages[n]:
            dest = os.path.normpath(os.path.join(t, relsrc))
            results.extend(env.InstallAs(dest, src))

    # Done
    return results


def InstallExclude(env, *args):
    env['INSTALLFILES_EXCLUDES'] = []

    for i in args:
        env['INSTALLFILES_EXCLUDES'].extend(Flatten(i))


# Register this with the environment
def TOOL_INSTALL(env):
    env.AddMethod(InstallFiles)
    env.AddMethod(InstallPackageAccum)
    env.AddMethod(InstallPackage)
    env.AddMethod(InstallExclude)

    env['INSTALLFILES_EXCLUDES'] = []
    env['INSTALLFILES_PACKAGES'] = {}
