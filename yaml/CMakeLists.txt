if(SEA_ENABLE_CRAB)
  configure_file(sea_crab_base.yaml sea.yaml @ONLY)
else()
  configure_file(sea.yaml sea.yaml @ONLY)
endif()
configure_file(sea_crab.yaml sea.crab.yaml @ONLY)
configure_file(sea_cex_base.yaml sea.cex.yaml @ONLY)
configure_file(clam_base.yaml clam.base.yaml @ONLY)
configure_file(sea_vac.yaml sea.vac.yaml @ONLY)
add_subdirectory(domain-yaml)