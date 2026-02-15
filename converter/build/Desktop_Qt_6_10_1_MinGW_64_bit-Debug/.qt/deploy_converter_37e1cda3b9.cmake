include("C:/qtt/converter/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/converter-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/qtt/converter/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/converter.exe"
    GENERATE_QT_CONF
)
