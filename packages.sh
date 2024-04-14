sudo pacman -Syu
sudo pacman -S libxinerama libx11 libxft xorg-server xorg-xinit xorg-xsetroot arandr kitty rofi feh picom dunst
sudo pacman -S lxappearance adapta-gtk-theme ttf-meslo-nerd ttf-nerd-fonts-symbols-mono code nano
sudo pacman -S network-manager-applet bluez spotify-launcher nemo payerctl pavucontrol blueberry
cd ~
git clone https://aur.archlinux.org/yay.git
cd yay
makepkg -si
yay -S breeze-light-cursor-theme papirus-icon-theme thorium-browser-bin
cd ~