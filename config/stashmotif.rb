# This is a short script that copies motif libraries into our local install
# directory. We only build motif stuff on systems that have the motif libraries,
# but Walt may then copy that directory structure to another machine
# that doesn't have motif installed. We only need the libraries, rather than
# all the headers because we have already built everything.

require "fileutils"

samplebin = ARGV[0]
installdir = ARGV[1]

copy_tree = lambda do |f|
  p f
  p File.dirname(f)
  p installdir
  unless(File.dirname(f) == installdir) # Skip if we are already getting this 
                                        # out of install directory
    if(File.symlink?(f))
      fname = installdir + "/" + File.basename(f)
      unless(f ==fname || File.exists?(fname)) 
        # We've already installed the libraries
        FileUtils.ln_s(File.basename(File.readlink(f)), fname)
        copy_tree.call(File.dirname(f) + "/" + File.readlink(f))
      end
    else
      FileUtils.copy(f,installdir)
    end
  end
end
[`ldd #{samplebin} | grep 'libXm\\.'`,
 `otool -L #{samplebin} | grep 'libXm\\.'`,
 `ldd #{samplebin} | grep 'libXp\\.'`,
 `otool -L #{samplebin} | grep 'libXp\\.'`].each do |f|
  f.sub!(/\s*\(.*/, "")
  f.sub!(/.*=>\s*/, "")
  f.strip!
  puts "'#{f}'" if(f =~ /\w/)
  copy_tree.call(f) if(f =~ /\w/)
end
