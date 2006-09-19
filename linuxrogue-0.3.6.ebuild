# Copyright 1999-2006 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

DESCRIPTION="linux port of the rogue role-playing game"
HOMEPAGE="http://nwserver.webhop.org/rogue.html"
SRC_URI="http://nwserver.webhop.org/${P}.tar.gz"

LICENSE="none"
SLOT="0"
KEYWORDS="x86"

DEPEND="sys-libs/ncurses"

src_compile() {
	make
}

src_install() {

