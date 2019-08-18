import React, { Component } from 'react';
import { withStyles } from '@material-ui/core/styles';
import PropTypes from 'prop-types';
import InputAdornment from '@material-ui/core/InputAdornment';
import Button from '@material-ui/core/Button';
import Typography from '@material-ui/core/Typography';
import LinearProgress from '@material-ui/core/LinearProgress';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';
import MenuItem from '@material-ui/core/MenuItem';
import Select from '@material-ui/core/Select';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Switch from '@material-ui/core/Switch';
import { Divider } from '@material-ui/core';

const styles = theme => ({
  root: {
    display: 'flex',
    flexWrap: 'wrap',
  },
  formControl: {
    margin: theme.spacing.unit,
    minWidth: 120,
  },
  formControlTypography: {
    minWidth: '100%'
  }
})

class BrewSettingsForm extends Component {

  render() {
    const { classes, brewSettings, errorMessage, handleValueChange, handleFloatValueChange, handleCheckboxChange, onSubmit } = this.props;

    return (
      <div>
        {
          !this.props.brewSettingsFetched ?
            <div>
              <LinearProgress />
              <Typography>
                Loading...
              </Typography>
            </div>
            : brewSettings ?
              <ValidatorForm onSubmit={onSubmit} ref="BrewSettingsForm" className={classes.root}>
                <TextValidator className={classes.formControl}
                  name="boilTime"
                  validators={['required']}
                  label="Boil time"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">min</InputAdornment> }}
                  value={brewSettings.boilTime}
                  onChange={handleValueChange("boilTime")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="boilTemperature"
                  validators={['required']}
                  label="Boil temperature"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                  value={brewSettings.boilTemperature}
                  onChange={handleValueChange("boilTemperature")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="boilPowerPercentage"
                  validators={['required']}
                  label="Boil Power %"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                  value={brewSettings.boilPowerPercentage}
                  onChange={handleValueChange("boilPowerPercentage")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="MashHeaterPercentage"
                  validators={['required']}
                  label="Mash Heater %"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                  value={brewSettings.mashHeaterPercentage}
                  onChange={handleValueChange("mashHeaterPercentage")}
                  errorMessages={['this field is required']}
                />
                <Typography className={classes.formControl} color="textSecondary">Main Sensor</Typography>
                <Select className={classes.formControl}
                  value={brewSettings.mainSensor}
                  onChange={handleValueChange("mainSensor")}
                  fullWidth
                  inputProps={{ required: true }}
                >
                  {this.props.sensors.map(value => (
                    <MenuItem value={value.address}>{value.address} - {value.value}ºC</MenuItem>
                  ))}
                </Select>
                <TextValidator className={classes.formControl}
                  name="MainSensorOffset"
                  validators={['required', 'isFloat']}
                  label="Main Sensor Offset (Calibration)"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                  value={brewSettings.mainSensorOffset}
                  onChange={handleFloatValueChange("mainSensorOffset")}
                  errorMessages={['this field is required']}
                />
                <FormControlLabel
                  control={
                    <Switch
                      checked={brewSettings.enableSparge}
                      value={'enableSparge'}
                      onChange={handleCheckboxChange('enableSparge')}
                      color="secondary"
                    />
                  }
                  label="Enable Sparge"
                />
                <Typography className={classes.formControlTypography} color="textSecondary">Sparge Sensor</Typography>
                <Select className={classes.formControl}
                  value={brewSettings.spargeSensor}
                  onChange={handleValueChange("spargeSensor")}
                  fullWidth
                  inputProps={{ required: true }}
                >
                  <option value=''></option>
                  {this.props.sensors.map(value => (
                    <MenuItem value={value.address}>{value.address} - {value.value}ºC</MenuItem>
                  ))}
                </Select>
                <TextValidator className={classes.formControl}
                  name="SpargeSensorOffset"
                  validators={['required', 'isFloat']}
                  label="Sparge Sensor Offset (Calibration)"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                  value={brewSettings.spargeSensorOffset}
                  onChange={handleFloatValueChange("spargeSensorOffset")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="spargePowerPercentage"
                  validators={['required']}
                  label="Sparge Power %"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                  value={brewSettings.spargePowerPercentage}
                  onChange={handleValueChange("spargePowerPercentage")}
                  errorMessages={['this field is required']}
                />

                <TextValidator className={classes.formControl}
                  name="spargeTemperature"
                  validators={['required', 'isFloat']}
                  label="Sparge Temperature"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                  value={brewSettings.spargeTemperature}
                  onChange={handleValueChange("spargeTemperature")}
                  errorMessages={['this field is required']}
                />

                <TextValidator className={classes.formControl}
                  name="pumpRestInterval"
                  validators={['required']}
                  label="Pump Rest Interval"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
                  value={brewSettings.pumpRestInterval}
                  onChange={handleValueChange("pumpRestInterval")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="pumpRestTime"
                  validators={['required']}
                  label="Pump Rest Time"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
                  value={brewSettings.pumpRestTime}
                  onChange={handleValueChange("pumpRestTime")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="kp"
                  label="kP"
                  validators={['required', 'isFloat']}
                  fullWidth
                  value={brewSettings.kP}
                  onChange={handleValueChange("kP")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="ki"
                  label="kI"
                  validators={['required', 'isFloat']}
                  fullWidth
                  value={brewSettings.kI}
                  onChange={handleValueChange("kI")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="kd"
                  label="kD"
                  validators={['required', 'isFloat']}
                  fullWidth
                  value={brewSettings.kD}
                  onChange={handleValueChange("kD")}
                  errorMessages={['this field is required']}
                />
                <TextValidator className={classes.formControl}
                  name="pidStart"
                  validators={['required', 'isFloat']}
                  label="PID Start at"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                  value={brewSettings.pidStart}
                  onChange={handleValueChange("pidStart")}
                  errorMessages={['this field is required']}
                />
                <Button variant="raised" color="secondary" type="submit">
                  Save
              </Button>
              </ValidatorForm>
              :
              <div>
                <Typography>
                  {errorMessage}
                </Typography>
              </div>
        }
      </div>
    )
  }
}

BrewSettingsForm.propTypes = {
  brewSettingsFetched: PropTypes.bool.isRequired,
  brewSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleCheckboxChange: PropTypes.func.isRequired,
  handleFloatValueChange: PropTypes.func.isRequired,
};

export default withStyles(styles)(BrewSettingsForm);