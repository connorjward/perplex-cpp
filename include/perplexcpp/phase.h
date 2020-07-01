
  /**
   * A struct containg phase information.
   */
  struct Phase
  {
    /**
     * The name of the phase used by Perple_X.
     */
    std::string standard_name;


    /**
     * The shortened form of the phase name.
     */
    std::string abbreviated_name;


    /**
     * The long form of the phase name.
     */
    std::string full_name;


    /**
     * The fractional amount of the phase (by weight).
     */
    double weight_frac;


    /**
     * The fractional amount of the phase (by volume).
     */
    double vol_frac;


    /**
     * The fractional amount of the phase (by molar amount).
     */
    double mol_frac;


    /**
     * The amount of the phase (mol).
     */
    double amount;


    /**
     * The phase composition.
     */
    std::vector<double> composition;
  };

