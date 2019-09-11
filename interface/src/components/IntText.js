import React, { Component } from 'react';
import T from 'i18n-react';
import en from "../language/en.json"
import ptBR from "../language/pt-BR.json"
import ruRU from "../language/ru-RU.json"

class IntText extends Component {
  constructor(props) {
    super(props)
  }

  SetText(lang) {
    switch (lang) {
      case "pt-BR":
        T.setTexts(ptBR)
        break
      case "en":
        T.setTexts(en);
        break
      case "ru-RU":
        T.setTexts(ruRU);
        break
      default:
        T.setTexts(en)
        break
    }
    this.forceUpdate()
  }

  render() {
    return (
      <span>
        {this.props.spaceBefore ? ' ' : null}
        <T.span text={this.props.text} />
        {this.props.spaceAfter ? ' ' : null}
      </span>
    )
  }
}

export default IntText;