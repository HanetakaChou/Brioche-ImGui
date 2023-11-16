#
# Copyright (C) YuqiaoZhang(HanetakaChou)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# https://developer.android.com/ndk/guides/android_mk

$(call import-module,build-linux)
$(call import-module,thirdparty/Brioche-ImGui/build-linux)
$(call import-module,thirdparty/Brioche-ImGui/thirdparty/freetype/build-linux)
$(call import-module,thirdparty/Brioche-Platform-Abstraction-Layer/build-linux)
$(call import-module,thirdparty/Brioche-Window-System-Integration/build-linux)
$(call import-module,thirdparty/McRT-Malloc/build-linux)
$(call import-module,thirdparty/McRT-Malloc/thirdparty/Intel-TBB/build-linux)
