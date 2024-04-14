# installs dwm with my .xinitrc status display

cd ~
sudo echo "$(cat dwm-trent/xinitrc)" > .xinitrc
cd dwm-titus
sudo make clean install
cd ~