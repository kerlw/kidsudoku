LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := kidsudoku_cpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/MainScene.cpp	\
                   ../../Classes/CampainScene.cpp	\
                   ../../Classes/RealTableView.cpp \
                   ../../Classes/StageScene.cpp	\
                   ../../Classes/StageData.cpp \
                   ../../Classes/CampaignData.cpp \
                   ../../Classes/NumberBar.cpp \
                   ../../Classes/SudokuBox.cpp \
                   ../../Classes/VictoryLayer.cpp \
                   ../../Classes/GameController.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
